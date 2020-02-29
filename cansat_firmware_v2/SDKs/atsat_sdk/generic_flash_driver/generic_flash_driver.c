#include "generic_flash_driver.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    ST_NOP,
    ST_WAITING_ERASE,
    ST_WAITING_PAGE_WRITE
} state_machine_t;

typedef enum {
    ULTRA_DEEP_POWER_DOWN_CMD = 0x79,
    READ_ID_CMD = 0x9F,
    CHIP_ERASE_CMD = 0x60,
    SECTOR_ERASE_4K_CMD = 0x20,
    READ_STATUS_CMD = 0x05,
    WRITE_STATUS_CMD = 0x01,
    PAGE_ERASE = 0x81,
    WRITE_ENABLE_CMD = 0x06,
    WRITE_DISABLE_CMD = 0x04,
    BYTE_PAGE_PROGRAM_CMD = 0x02,
    READ_ARRAY_CMD = 0x0B,
    READ_SECTOR_PROTECTION_CMD = 0x3C
} flash_commands_t;

// State machine
static volatile state_machine_t state;
static bool error_ocurred;
static generic_flash_driver_t *driver;

bool generic_flash_driver_init(generic_flash_driver_t *cfg)
{
    driver = cfg;
    if(!driver->init()) {
        return false;
    }

    driver->set_cs(true);
    driver->set_wp(true);
    state = ST_NOP;

    // Globally unprotect the entire memory
    generic_flash_driver_write_status(0);

    // Put memory to sleep
    generic_flash_driver_enter_deep_power_down();

    return true;
}

void generic_flash_driver_tasks(void)
{
    uint8_t status;

    switch (state) {
        case ST_NOP:
            break;

        // Improvement possibility. Instead of polling the status byte, use the Active Status Interrupt
        //  command to generate an interrupt when the erasing is complete.
        case ST_WAITING_ERASE:
            // Get the status to know if the erasing is complete
            if (generic_flash_driver_read_status(&status)) {
                // If not busy, erase operation finished
                if (!(status & 0x01)) {
                    state = ST_NOP;
                }
                else {
                    break;
                }
            }
            break;

        case ST_WAITING_PAGE_WRITE:
            if (generic_flash_driver_read_status(&status)) {
                // If not busy, writing operation finished
                if (!(status & 0x01)) {
                    generic_flash_driver_block_writes();
                    state = ST_NOP;
                }
            }
            break;
    }
}

bool generic_flash_driver_write_status(unsigned int value)
{
    if (ST_NOP) {
        generic_flash_driver_exit_deep_power_down();
        generic_flash_driver_allow_writes();

        driver->set_cs(true);
        driver->set_wp(true);

        // Send status write command
        driver->exchange_byte(WRITE_STATUS_CMD);

        // Now send the value to write to the status register byte 1
        driver->exchange_byte(value);

        driver->set_cs(true);
        driver->set_wp(false);

        return true;
    }

    return false;
}

/**
 * Read the status byte from the memory
 * @param status pointer where to store the status register
 * @return true if the read was executed, false otherwise
 */
bool generic_flash_driver_read_status(uint8_t* status)
{
    if (ST_NOP || ST_WAITING_ERASE || ST_WAITING_PAGE_WRITE) {
        generic_flash_driver_exit_deep_power_down();

        driver->set_cs(false);
        driver->set_wp(true);

        // Send status read command
        driver->exchange_byte(READ_STATUS_CMD);

        // Now read the status register
        *status = driver->exchange_byte(0x00);

        driver->set_cs(true);
        driver->set_wp(false);

        return true;
    }

    return false;
}

bool generic_flash_driver_is_page_protected(uint32_t address)
{
    if (ST_NOP) {
        generic_flash_driver_exit_deep_power_down();

        driver->set_cs(false);
        driver->set_wp(false);

        // Send read sector protection command
        driver->exchange_byte(READ_SECTOR_PROTECTION_CMD);

        // Now send the 3 byte address starting with the MSB
        driver->exchange_byte((uint8_t)(address >> 16));
        driver->exchange_byte((uint8_t)(address >> 8));
        driver->exchange_byte((uint8_t)address);

        // Read the protection status byte
        uint8_t status = driver->exchange_byte(0x00);

        driver->set_cs(true);
        driver->set_wp(false);

        if (status == 0xFF) {
            return true;
        }
    }

    return false;
}

/**
 * Enable writes on the memory
 * @return true if the command was executed, false otherwise
 */
bool generic_flash_driver_allow_writes(void)
{
    if (state == ST_NOP) {
        generic_flash_driver_exit_deep_power_down();

        driver->set_cs(false);
        driver->set_wp(true);

        // Send write enable command
        driver->exchange_byte(WRITE_ENABLE_CMD);

        driver->set_cs(true);
        driver->set_wp(false);

        return true;
    }

    return false;
}

bool generic_flash_driver_block_writes(void)
{
    return false;
}

/**
 * Erase the entire memory. This takes some seconds so be careful.
 * This method runs async so when it returns the erasing process is not
 *  complete. Use generic_flash_driver_is_erasing() to determine if the erasing process
 *  was completed.
 * @return 
 */
bool generic_flash_driver_chip_erase(void)
{
    if (state == ST_NOP) {
        generic_flash_driver_exit_deep_power_down();
        // When exiting ultra-deep power-down the register are reset to they
        //  power-up state so we have to unprotect the memory again to be able to
        //  write to it and erase it
        generic_flash_driver_write_status(0);

        if (!generic_flash_driver_allow_writes()) {
            return false;
        }

        driver->set_cs(false);
        driver->set_wp(true);

        // Send chip erase command
        driver->exchange_byte(CHIP_ERASE_CMD);

        state = ST_WAITING_ERASE;
        error_ocurred = false;

        driver->set_cs(true);
        driver->set_wp(false);

        return true;
    }

    return false;
}

bool generic_flash_driver_4K_block_erase(uint16_t block_number)
{
    if (state == ST_NOP) {
        generic_flash_driver_exit_deep_power_down();
        // When exiting ultra-deep power-down the register are reset to they
        //  power-up state so we have to unprotect the memory again to be able to
        //  write to it and erase it
        generic_flash_driver_write_status(0);

        if (!generic_flash_driver_allow_writes()) {
            return false;
        }

        // Calculate address of the block
        uint32_t address = (uint32_t)block_number * 4096;

        driver->set_cs(false);
        driver->set_wp(true);

        // Send the command and the address first starting with the MSB
        driver->exchange_byte(SECTOR_ERASE_4K_CMD);
        driver->exchange_byte((uint8_t)((address & 0x00FF0000) >> 16));
        driver->exchange_byte((uint8_t)((address & 0x0000FF00) >> 8));
        driver->exchange_byte((uint8_t)address);

        state = ST_WAITING_ERASE;
        error_ocurred = false;

        driver->set_cs(true);
        driver->set_wp(false);

        return true;
    }

    return false;
}

bool generic_flash_driver_32K_block_erase(uint16_t block_number)
{
    (void)block_number;
    return false;
}

bool generic_flash_driver_64K_block_erase(uint16_t block_number)
{
    (void)block_number;
    return false;
}

/**
 * Checks if some kind of erase operation is in progress
 * @return true if some kind of erase operation is in progress
 */
bool generic_flash_driver_is_erasing(void)
{
    return state == ST_WAITING_ERASE;
}

/**
 * Program a page with the given data. This method send the bytes to the flash memory but does NOT wait
 *  for them to be written. To check when the page has finished writting check the status with 
 *  generic_flash_driver_is_writing().
 * @param data array of data to write
 * @param length number of bytes to write
 * @param page_number page number to write, starting from 0
 * @return true if the writing was started, false otherwise.
 */
bool generic_flash_driver_page_write(uint8_t* data, unsigned int length, unsigned int page_number, unsigned int offset)
{
    if (state == ST_NOP && length > 0) {
        generic_flash_driver_exit_deep_power_down();

        // When exiting ultra-deep power-down the register are reset to they
        //  power-up state so we have to unprotect the memory again to be able to
        //  write to it and erase it
        generic_flash_driver_write_status(0);
        
        if (!generic_flash_driver_allow_writes()) {
            return false;
        }

        if (length > 256) {
            length = 256;
        }

        if (page_number > driver->max_pages) {
            page_number = driver->max_pages;
        }

        // Calculate address of the page
        uint32_t address = (uint32_t)page_number * 256 + offset;

        driver->set_cs(false);
        driver->set_wp(true);

        // Send the command and the address first starting with the MSB
        driver->exchange_byte(BYTE_PAGE_PROGRAM_CMD);
        driver->exchange_byte((uint8_t)((address & 0x00FF0000) >> 16));
        driver->exchange_byte((uint8_t)((address & 0x0000FF00) >> 8));
        driver->exchange_byte((uint8_t)address);

        // Now send the data bytes
        driver->write_bytes(data, length);

        // Finished writing data
        driver->set_cs(true);
        driver->set_wp(false);
        state = ST_WAITING_PAGE_WRITE;

        return true;
    }

    return false;
}

/**
 * Read data from a memory page
 * @param out pointer to the array where to store the data
 * @param length number of bytes to read
 * @param page_number page number to read, starting from 0
 * @param offset offset byte from the beginning of the page (from 0 to 254). Use the offset
 *  to start reading at any other position than the beginning
 * @return 
 */
bool generic_flash_driver_read(uint8_t* out, unsigned int length, unsigned int page_number, unsigned int offset)
{
    if (state == ST_NOP && length > 0) {
        generic_flash_driver_exit_deep_power_down();

        driver->set_cs(false);
        driver->set_wp(false);

        if (page_number > driver->max_pages) {
            page_number = driver->max_pages;
        }

        // Calculate address
        uint32_t address = (uint32_t)page_number * 256 + offset;

        // Send read array command
        driver->exchange_byte(READ_ARRAY_CMD);

        // Send the 3 byte address starting with the MSB
        driver->exchange_byte((uint8_t)((address & 0x00FF0000) >> 16));
        driver->exchange_byte((uint8_t)((address & 0x0000FF00) >> 8));
        driver->exchange_byte((uint8_t)address);

        // Send the dummy byte
        driver->exchange_byte(0x00);

        // Read the data now
        driver->read_bytes(out, length);

        // Finish receiving bytes
        driver->set_cs(true);
        driver->set_wp(false);

        return true;
    }

    return false;
}

/**
 * Puts the memory in ultra deep power down mode
 * @return true if the command was executed, false otherwise
 */
bool generic_flash_driver_enter_deep_power_down(void)
{
    return true;
}

/**
 * Check if there was an error in the last erase/write operation
 * @return true if there was an error in last erase/write operation
 */
bool generic_flash_driver_is_error(void)
{
    return error_ocurred;
}

/**
 * Wakes the memory from sleep
 * @return true if the command was executed, false otherwise
 */
bool generic_flash_driver_exit_deep_power_down(void)
{
    return true;
}

/**
 * Returns true if a writing operation is in progress
 * @return true if a writing operation is in progress, false otherwise
 */
bool generic_flash_driver_is_writing(void)
{
    return state == ST_WAITING_PAGE_WRITE;
}

uint32_t generic_flash_driver_read_id(void)
{
    if (state == ST_NOP) {
        uint8_t temp[3];
        generic_flash_driver_exit_deep_power_down();

        driver->set_cs(false);
        driver->set_wp(false);

        // Send read ID command
        driver->exchange_byte(READ_ID_CMD);

        // Read the 3 bytes
        driver->read_bytes(temp, 3);

        // Finish receiving bytes
        driver->set_cs(true);
        driver->set_wp(false);

        uint32_t id = (((uint32_t)temp[2]) << 16) | (((uint32_t)temp[1]) << 8) | temp[0];
        return id;
    }

    return 0;
}