#ifndef __MX25R6435F_DRIVER_H__
#define __MX25R6435F_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    // Function to initialize the HAL driver
    bool (*init)(void);

    // Function to set or clear the CS pin
    void (*set_cs)(bool state);

    // Function to set or clear the WP pin
    void (*set_wp)(bool state);

    // Transfer a byte through SPI and read the returned byte
    uint8_t (*exchange_byte)(uint8_t data);

    // Write multiple bytes to the SPI interface
    void (*write_bytes)(uint8_t *data, unsigned int length);

    // Read multiple bytes from the SPI interface
    void (*read_bytes)(uint8_t *out, unsigned int length);

    // Maximum number of pages in memory
    uint32_t max_pages;
} generic_flash_driver_t;

bool generic_flash_driver_init(generic_flash_driver_t *cfg);
void generic_flash_driver_tasks(void);

bool generic_flash_driver_write_status(unsigned int value);
bool generic_flash_driver_read_status(uint8_t* status);
bool generic_flash_driver_is_page_protected(uint32_t address);
bool generic_flash_driver_allow_writes(void);
bool generic_flash_driver_block_writes(void);

// Erase methods
bool generic_flash_driver_chip_erase(void);
bool generic_flash_driver_4K_block_erase(uint16_t block_number);
bool generic_flash_driver_32K_block_erase(uint16_t block_number);
bool generic_flash_driver_64K_block_erase(uint16_t block_number);
bool generic_flash_driver_is_erasing(void);

// Read/Write methods
bool generic_flash_driver_page_write(uint8_t* data, unsigned int length, unsigned int page_number, unsigned int offset);
bool generic_flash_driver_read(uint8_t* out, unsigned int length, unsigned int page_number, unsigned int offset);
bool generic_flash_driver_is_writing(void);
uint32_t generic_flash_driver_read_id(void);

bool generic_flash_driver_is_error(void);
bool generic_flash_driver_enter_deep_power_down(void);
bool generic_flash_driver_exit_deep_power_down(void);

#ifdef __cplusplus
}
#endif

#endif