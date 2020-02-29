#include "ext_flash_manager.h"
#include "littlefs/lfs.h"
#include "generic_flash_driver.h"
#include "nrf_log.h"
#include "ext_flash_manager_config.h"
#include <stdbool.h>

// Prototipes
int flash_block_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int flash_block_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int flash_block_erase(const struct lfs_config *c, lfs_block_t block);
int flash_block_sync(const struct lfs_config *c);

static lfs_t lfs;
static lfs_file_t log_file, data_file;
static bool initialized = false;
static bool log_initialized = false;
static ext_flash_manager_cfg_t *config;

// LittleFS buffers
static uint8_t read_buffer[EXT_FLASH_MANAGER_LFS_CACHE_SIZE];
static uint8_t prog_buffer[EXT_FLASH_MANAGER_LFS_CACHE_SIZE];
static uint8_t look_ahead_buffer[EXT_FLASH_MANAGER_LFS_LOOK_AHEAD_SIZE] __attribute__ ((aligned (8)));

// LittleFS configuration
static const struct lfs_config lfs_cfg = {
    // block device operations
    .read  = flash_block_read,
    .prog  = flash_block_prog,
    .erase = flash_block_erase,
    .sync  = flash_block_sync,

    // block device configuration
    .read_size = EXT_FLASH_MANAGER_LFS_READ_SIZE,
    .prog_size = EXT_FLASH_MANAGER_LFS_PROG_SIZE,
    .block_size = EXT_FLASH_MANAGER_LFS_BLOCK_SIZE,
    .block_count = EXT_FLASH_MANAGER_BLOCK_COUNT,
    .block_cycles = EXT_FLASH_MANAGER_BLOCK_CYCLES,
    .cache_size = EXT_FLASH_MANAGER_LFS_CACHE_SIZE,
    .lookahead_size = EXT_FLASH_MANAGER_LFS_LOOK_AHEAD_SIZE,

    .read_buffer = read_buffer,
    .prog_buffer = prog_buffer,
    .lookahead_buffer = look_ahead_buffer,
    .name_max = EXT_FLASH_MANAGER_LFS_NAME_MAX
};

typedef struct {
    uint32_t sample_count;          // Number of samples stored
    uint32_t last_written_sample;   // Last written sample
    uint8_t sample_size;            // Size of every sample in the file
} data_file_header_t;


// LittleFS HAL functions
int flash_block_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    if(generic_flash_driver_read((uint8_t *)buffer, size, ((block + EXT_FLASH_MANAGER_LFS_STARTING_BLOCK) * c->block_size) / 256, off)) {
        return LFS_ERR_OK;
    }
    return LFS_ERR_IO;
}

int flash_block_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    const unsigned offset_in_pages = (off / 256);
    const unsigned int starting_page_number = ((block + EXT_FLASH_MANAGER_LFS_STARTING_BLOCK) * c->block_size) / 256 + offset_in_pages;
    unsigned int offset = off - (offset_in_pages * 256);

    // We need to do multiple transfers for more than 256 bytes (a page) because the flash memory only allows
    //  writing a page at a time.
    for(unsigned int n = 0; n < size + offset;) {
        const unsigned int page_number = starting_page_number + (n / 256);

        // Calculate the number of bytes to send in this chunk
        unsigned int transfer_size = size - n - offset;
        if(transfer_size > 256) {
            transfer_size = 256 - offset;
        }

        // Write the corresponding page
        if(generic_flash_driver_page_write((uint8_t *)buffer, transfer_size, page_number, offset)) {
            while(generic_flash_driver_is_writing()) {
                generic_flash_driver_tasks();
            }
        }
        else {
            return LFS_ERR_IO;
        }

        n += transfer_size + offset;

        // Offset is only applied on the first programmed page
        offset = 0;
    }

    return LFS_ERR_OK;
}

int flash_block_erase(const struct lfs_config *c, lfs_block_t block)
{
    (void)c;

    if(generic_flash_driver_4K_block_erase(block + EXT_FLASH_MANAGER_LFS_STARTING_BLOCK)) {
        while(generic_flash_driver_is_erasing()) {
            generic_flash_driver_tasks();
        }
        return LFS_ERR_OK;
    }
    return LFS_ERR_IO;
}

int flash_block_sync(const struct lfs_config *c)
{
    (void)c;

    // Our flash memory doesn't have a cache so sync does nothing
    return LFS_ERR_OK;
}



bool ext_flash_manager_init(ext_flash_manager_cfg_t * const cfg) {
    if(!initialized) {
        ASSERT(cfg);
        ASSERT(cfg->max_log_size < (cfg->flash_driver.max_pages * 256));
        ASSERT(cfg->max_data_size < (cfg->flash_driver.max_pages * 256));
        ASSERT(cfg->max_data_size > (sizeof(data_file_header_t) + EXT_FLASH_MANAGER_DATA_SIZE + 1));

        config = cfg;

        // Initialize the external flash driver
        if(!generic_flash_driver_init(&config->flash_driver)) {
            NRF_LOG_ERROR("Failed initializing flash driver");
            return false;
        }

        // Mount the filesystem
        int err = lfs_mount(&lfs, &lfs_cfg);

        // Reformat if we can't mount the filesystem,
        //  this should only happen on the first boot
        if (err) {
            if((err = lfs_format(&lfs, &lfs_cfg))) {
                // TODO: Severe error!
                return false;
            }

            if((err = lfs_mount(&lfs, &lfs_cfg))) {
                // TODO: Severe error!
                return false;
            }
        }

        // Open the data file
        if((err = lfs_file_open(&lfs, &data_file, EXT_FLASH_MANAGER_DATA_FILENAME, LFS_O_RDWR | LFS_O_APPEND))) {
            // File does not exists, create and initialize it
            if(err == LFS_ERR_NOENT) {
                #ifdef DEBUG
                    NRF_LOG_INFO("Creating data file");
                #endif
                
                // Create the non-existent file and initialize it.
                if((err = lfs_file_open(&lfs, &data_file, EXT_FLASH_MANAGER_DATA_FILENAME, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND))) {
                    #ifdef DEBUG
                        NRF_LOG_ERROR("Error creating data file: %d", err);
                    #endif
                    return false;
                }

                // Write the file header
                data_file_header_t header = {
                    .sample_count = 0,
                    .last_written_sample = 0,
                    .sample_size = EXT_FLASH_MANAGER_DATA_SIZE
                };
                if((err = lfs_file_write(&lfs, &data_file, (void *)&header, sizeof(header))) < 0) {
                    #ifdef DEBUG
                        NRF_LOG_ERROR("Error initializing data file: %d", err);
                    #endif
                    return false;
                }

                if((err = lfs_file_sync(&lfs, &data_file))) {
                    #ifdef DEBUG
                        NRF_LOG_ERROR("Error synchronizing data file: %d", err);
                    #endif
                    return false;
                }
            }
            // Error
            else {
                #ifdef DEBUG
                    NRF_LOG_ERROR("Error opening log file: %d", err);
                #endif
                return false;
            }
        }

        initialized = true;
    }

    return true;
}

bool ext_flash_manager_is_initialized(void) {
    return initialized;
}

bool ext_flash_manager_start_log(void) {
    if(initialized) {
        if(lfs_file_open(&lfs, &log_file, EXT_FLASH_MANAGER_LOG_FILENAME, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND)) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error opening log file");
            #endif
            return false;
        }

        log_initialized = true;
        return true;
    }

    return false;
}

void ext_flash_manager_close_log(void) {
    if(initialized) {
        int err;

        if((err = lfs_file_close(&lfs, &log_file))) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error closing log file: %d", err);
            #endif
        }

        log_initialized = false;
    }
}

bool ext_flash_manager_sync_log(void) {
    if(initialized && log_initialized) {
        int err;

        if((err = lfs_file_sync(&lfs, &log_file))) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error synchronizing log file: %d", err);
            #endif
            return false;
        }

        return true;
    }

    return false;
}

bool ext_flash_manager_write_log(char * const log, unsigned int length) {
    if(initialized && log_initialized) {
        int err;
        int32_t size;

        if((size = lfs_file_size(&lfs, &log_file)) < 0) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error getting log file size: %d", size);
            #endif
            return false;
        }

        // Check if we don't go over the max log size
        if(size + length > config->max_log_size) {
            NRF_LOG_INFO("Swapping log files");

            if(lfs_file_close(&lfs, &log_file)) {
                #ifdef DEBUG
                    NRF_LOG_ERROR("Error closing log file: %d", err);
                #endif
                return false;
            }

            // Rename the current log file and create a new one so we can rotate between two log files
            lfs_remove(&lfs, EXT_FLASH_MANAGER_OLD_LOG_FILENAME);
            lfs_rename(&lfs, EXT_FLASH_MANAGER_LOG_FILENAME, EXT_FLASH_MANAGER_OLD_LOG_FILENAME);

            // Now open the log file again truncating it
            if(lfs_file_open(&lfs, &log_file, EXT_FLASH_MANAGER_LOG_FILENAME, LFS_O_RDWR | LFS_O_CREAT | LFS_O_TRUNC)) {
                #ifdef DEBUG
                    NRF_LOG_ERROR("Error opening log file on rename: %d", err);
                #endif
                return false;
            }
        }

        if((err = lfs_file_write(&lfs, &log_file, (void *)log, length)) < 0) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error writing log file: %d", err);
            #endif
            return false;
        }

        return true;
    }

    return false;
}

unsigned int ext_flash_manager_read_log(char* log, unsigned int length, unsigned int offset) {
    if(initialized && log_initialized) {
        int err, read_bytes;

        // Position the offset
        if((err = lfs_file_seek(&lfs, &log_file, offset, LFS_SEEK_SET)) < 0) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error seeking offset for log file: %d", err);
            #endif
            return 0;
        }

        // Read the file
        if((read_bytes = lfs_file_read(&lfs, &log_file, (void *)log, length)) < 0) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error reading log file: %d", read_bytes);
            #endif
            return 0;
        }

        // Return the file position to the end
        if((err = lfs_file_seek(&lfs, &log_file, 0, LFS_SEEK_END)) < 0) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error setting offset for log file: %d", err);
            #endif
            return 0;
        }

        return read_bytes;
    }

    return 0;
}

unsigned int ext_flash_manager_read_old_log(char* log, unsigned int length, unsigned int offset) {
    if(initialized && log_initialized) {
        int err, read_bytes;
        lfs_file_t file;

        // Open the old log
        if(lfs_file_open(&lfs, &file, EXT_FLASH_MANAGER_LOG_FILENAME, LFS_O_RDONLY)) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error opening old log file: %d", err);
            #endif
            return 0;
        }

        // Position the offset
        if((err = lfs_file_seek(&lfs, &file, offset, LFS_SEEK_SET)) < 0) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error seeking offset for old log file: %d", err);
            #endif
            lfs_file_close(&lfs, &file);
            return 0;
        }

        // Read the file
        if((read_bytes = lfs_file_read(&lfs, &file, (void *)log, length)) < 0) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error reading old log file: %d", read_bytes);
            #endif
            lfs_file_close(&lfs, &file);
            return 0;
        }

        // Close the file
        if((err = lfs_file_close(&lfs, &file)) < 0) {
            #ifdef DEBUG
                NRF_LOG_ERROR("Error closing old log file: %d", err);
            #endif
            return 0;
        }

        return read_bytes;
    }

    return 0;
}

bool ext_flash_manager_write_settings(uint8_t * const settings, unsigned int size) {
    if(initialized) {
        lfs_file_t file;
        int err;

        if((err = lfs_file_open(&lfs, &file, EXT_FLASH_MANAGER_SETTINGS_FILENAME, LFS_O_WRONLY | LFS_O_CREAT))) {
            NRF_LOG_ERROR("Error opening settings file: %d", err);
            return false;
        }

        // Write the entire settings in the file overwriting everything that is currently in it
        if((err = lfs_file_write(&lfs, &file, (void *)settings, size))) {
            NRF_LOG_ERROR("Error writing settings file: %d", err);
            lfs_file_close(&lfs, &file);
            return false;
        }

        lfs_file_close(&lfs, &file);
        return true;
    }

    return false;
}

bool ext_flash_manager_read_settings(uint8_t * const out, unsigned int size) {
    if(initialized) {
        lfs_file_t file;
        int err;

        if((err = lfs_file_open(&lfs, &file, EXT_FLASH_MANAGER_SETTINGS_FILENAME, LFS_O_RDONLY))) {
            NRF_LOG_ERROR("Error opening settings file: %d", err);
            return false;
        }

        if((err = lfs_file_read(&lfs, &file, (void *)out, size))) {
            NRF_LOG_ERROR("Error reading settings file: %d", err);
            lfs_file_close(&lfs, &file);
            return false;
        }

        lfs_file_close(&lfs, &file);
        return true;
    }

    return false;
}

bool ext_flash_manager_add_data(uint8_t * const data, uint8_t size) {
    if(initialized) {
        int32_t err;
        data_file_header_t header;

        // Read the file header
        if((err = lfs_file_seek(&lfs, &data_file, 0, LFS_SEEK_SET)) < 0) {
            NRF_LOG_ERROR("Error seeking start of data file: %d", err);
            return false;
        }
        if((err = lfs_file_read(&lfs, &data_file, (void *)&header, sizeof(header))) < 0) {
            NRF_LOG_ERROR("Error reading data file header: %d", err);
            return false;
        }

        // Check file size
        if((err = lfs_file_size(&lfs, &data_file)) < 0) {
            NRF_LOG_ERROR("Error getting data file size: %d", err);
            return false;
        }

        if(size > header.sample_size) {
            return false;
        }

        // If we get over the max file size start overwriting the samples at the
        //  beginning of the file
        if(((uint32_t)err + size + 1) > config->max_data_size) {
            // If sample count and last written sample are the same then move to the top again
            if(header.last_written_sample == (header.sample_count - 1)) {
                header.last_written_sample = 0;
            }
            else {
                ++header.last_written_sample;
            }

            // Calculate the file position to write the new sample
            int32_t file_position = sizeof(header) + header.last_written_sample * (header.sample_size + 1);

            if((err = lfs_file_seek(&lfs, &data_file, file_position, LFS_SEEK_SET)) < 0) {
                NRF_LOG_ERROR("Error seeking position for overwrite of data file: %d, %d", err, file_position);
                return false;
            }
        }
        // Otherwise just move to the end of the file
        else {
            if((err = lfs_file_seek(&lfs, &data_file, 0, LFS_SEEK_END)) < 0) {
                NRF_LOG_ERROR("Error seeking end of data file: %d", err);
                return false;
            }

            ++header.last_written_sample;
            ++header.sample_count;
        }

        // Add the sample now. The sample is stored as 1-byte indicating the size of the sample followed by the
        //  sample data and filling the remaining bytes with 0xFF to reach the EXT_FLASH_MANAGER_DATA_SIZE size
        if((err = lfs_file_write(&lfs, &data_file, (void *)&size, 1)) < 0) {
            NRF_LOG_ERROR("Error writing sample size: %d", err);
            return false;
        }

        if((err = lfs_file_write(&lfs, &data_file, (void *)data, size)) < 0) {
            NRF_LOG_ERROR("Error writing sample data: %d", err);
            return false;
        }

        // Fill remaining bytes with 0xFF
        uint8_t filler = 0xFF;
        for(unsigned int n = 0; n < (unsigned)(header.sample_size - size); ++n) {
            if((err = lfs_file_write(&lfs, &data_file, (void *)&filler, 1)) < 0) {
                NRF_LOG_ERROR("Error writing fillers: %d", err);
                return false;
            }
        }

        // Write the new file header now
        if((err = lfs_file_seek(&lfs, &data_file, 0, LFS_SEEK_SET)) < 0) {
            NRF_LOG_ERROR("Error seeking start of data file: %d", err);
            return false;
        }
        if((err = lfs_file_write(&lfs, &data_file, (void *)&header, sizeof(header))) < 0) {
            NRF_LOG_ERROR("Error updating data file counter: %d", err);
            return false;
        }

        // Flush the file and we are done!
        if((err = lfs_file_sync(&lfs, &data_file)) < 0) {
            NRF_LOG_ERROR("Error synchronizing data file: %d", err);
            return false;
        }

        return true;
    }

    return false;
}

bool ext_flash_manager_read_data(uint8_t * const out, unsigned int sample_number, uint8_t * const read_bytes) {
    if(initialized) {
        uint8_t temp[4], sample_size;
        int err;

        // Read the samples count
        if((err = lfs_file_seek(&lfs, &data_file, 0, LFS_SEEK_SET)) < 0) {
            NRF_LOG_ERROR("Error seeking start of data file: %d", err);
            return false;
        }

        if((err = lfs_file_read(&lfs, &data_file, (void *)temp, sizeof(temp))) < 0) {
            NRF_LOG_ERROR("Error reading data file count: %d", err);
            return false;
        }

        uint32_t current_count = (((uint32_t)temp[0]) << 24) | (((uint32_t)temp[1]) << 16) | (((uint32_t)temp[2]) << 8) | temp[3];
        ++current_count;

        // Sample non-existent
        if(sample_number > current_count) {
            return false;
        }
        
        // Calculate the offset where the sample is located:
        //  - 4 is because of the first 4 bytes from the sample counter
        //  - (EXT_FLASH_MANAGER_DATA_SIZE + 1) is because before any sample there's a byte indicating the sample size
        uint32_t offset = 4 + sample_number * (EXT_FLASH_MANAGER_DATA_SIZE + 1);

        if((err = lfs_file_seek(&lfs, &data_file, offset, LFS_SEEK_SET)) < 0) {
            NRF_LOG_ERROR("Error seeking start of sample: %d", err);
            return false;
        }

        // Get the sample size
        if((err = lfs_file_read(&lfs, &data_file, (void *)&sample_size, 1)) < 0) {
            NRF_LOG_ERROR("Error reading sample size: %d", err);
            return false;
        }

        // Corrupted sample
        if(sample_size > EXT_FLASH_MANAGER_DATA_SIZE) {
            NRF_LOG_ERROR("Corrupted sample with size %u", sample_size);
            return false;
        }
    
        // Read the sample data now
        if((err = lfs_file_seek(&lfs, &data_file, offset + 1, LFS_SEEK_SET)) < 0) {
            NRF_LOG_ERROR("Error seeking start of sample data: %d", err);
            return false;
        }

        if((err = lfs_file_read(&lfs, &data_file, (void *)out, sample_size)) < 0) {
            NRF_LOG_ERROR("Error reading sample: %d", err);
            return false;
        }

        *read_bytes = sample_size;
        return true;
    }

    return false;
}