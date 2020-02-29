#ifndef __EXT_FLASH_MANAGER_H__
#define __EXT_FLASH_MANAGER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "generic_flash_driver.h"

typedef struct {
    // Flash driver configuration
    generic_flash_driver_t flash_driver;
    
    // Max size in bytes of the log file. The total amount of space used by the logs will be up to
    //  2x'max_log_size' because logs are rotated between two files. 
    uint32_t max_log_size;

    // Max size in bytes for the data file. When the max file size is reached, old entries are overwrited.
    uint32_t max_data_size;
} ext_flash_manager_cfg_t;

/**
 * Initialize the external flash manager. This manages the access to the external flash memory to be able
 *  to use files in the LittleFS filesystem.
 * @param ext_flash_manager_cfg_t cfg pointer to configuration structure.
 * @returns true if init was successful, false otherwise.
 */
bool ext_flash_manager_init(ext_flash_manager_cfg_t * const cfg);

/**
 * Check if the external flash manager is ready to be used.
 * @returns true if the manager can be used.
 */
bool ext_flash_manager_is_initialized(void);

/**
 * Open and initialize the file that is used for logging purposes. This must be called before using log functions.
 * @returns true if log file was started successfully.
 */
bool ext_flash_manager_start_log(void);

/**
 * Close and sync the log file.
 */
void ext_flash_manager_close_log(void);

/**
 * Synchronizes the buffers from the log file to the external memory.
 */
bool ext_flash_manager_sync_log(void);

/**
 * Write to the log file.
 * @param char* log string to put in the log file.
 * @param unsigned int length size of the given string.
 * @returns true if write was successful, false otherwise.
 */
bool ext_flash_manager_write_log(char * const log, unsigned int length);

/**
 * Read from the log file.
 * @param char* log buffer to store the string from the log file. The string MAY NOT be terminated with a null character.
 * @param unsigned int length number of bytes to read from the log file.
 * @param unsigned int offset offset from the beginning of the log file to start reading.
 * @returns number of read bytes.
 */
unsigned int ext_flash_manager_read_log(char* log, unsigned int length, unsigned int offset);

/**
 * Read from the old log file.
 * @param char* log buffer to store the string from the old log file. The string MAY NOT be terminated with a null character.
 * @param unsigned int length number of bytes to read from the log file.
 * @param unsigned int offset offset from the beginning of the log file to start reading.
 * @returns number of read bytes.
 */
unsigned int ext_flash_manager_read_log(char* log, unsigned int length, unsigned int offset);

/**
 * Write settings to the settings file.
 * @param uint8_t* pointer to the settings array or struct to store.
 * @param unsigned int size number of bytes to write.
 * @returns true if operation was successful.
 */
bool ext_flash_manager_write_settings(uint8_t * const settings, unsigned int size);

/**
 * Read settings from the settings file.
 * @param uint8_t* out pointer to struct or array to store the settings from the file.
 * @param unsigned int size number of bytes to read.
 * @returns true if operation was successful.
 */
bool ext_flash_manager_read_settings(uint8_t * const out, unsigned int size);

/**
 * Add a sample to the data file. The samples can be up to 'EXT_FLASH_MANAGER_DATA_SIZE' in size.
 * @param uint8_t* data data to add into the file.
 * @param unsigned int size size of the data to add.
 * @returns true if data was saved.
 */
bool ext_flash_manager_add_data(uint8_t * const data, uint8_t size);

/**
 * Read a sample from the data file.
 * @param uint8_t* out pointer to the location to store the sample.
 * @param unsigned int sample_number number of sample to retrieve, starting with 0.
 * @param uint8_t* read_bytes number of bytes that were read for this sample.
 * @returns true if sample was read.
 */
bool ext_flash_manager_read_data(uint8_t * const out, unsigned int sample_number, uint8_t * const read_bytes);

#ifdef __cplusplus
}
#endif

#endif