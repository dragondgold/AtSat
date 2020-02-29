#include "flash_hal.h"
#include "app_error.h"
#include "legacy/nrf_drv_spi.h"
#include "nrf_gpio.h"
#include "nrf_log.h"

#ifdef BOARD_PCA10056
    #define SCK_PIN         NRF_GPIO_PIN_MAP(0, 19)
    #define MOSI_PIN        NRF_GPIO_PIN_MAP(0, 20)
    #define MISO_PIN        NRF_GPIO_PIN_MAP(0, 21)
    #define CS_PIN          NRF_GPIO_PIN_MAP(0, 17)
    #define WP_PIN          NRF_GPIO_PIN_MAP(0, 22)
    #define HOLD_PIN        NRF_GPIO_PIN_MAP(0, 23)
#else
    #error "Define flash memory pins here"
#endif

static const nrf_drv_spi_t spi_master = NRF_DRV_SPI_INSTANCE(0);

bool flash_hal_init(void)
{
    // Initialize the SPI driver
    nrf_drv_spi_config_t config = NRF_DRV_SPI_DEFAULT_CONFIG;
    config.sck_pin = SCK_PIN;
    config.mosi_pin = MOSI_PIN;
    config.miso_pin = MISO_PIN;

    config.frequency = NRF_DRV_SPI_FREQ_8M;
    config.mode = NRF_DRV_SPI_MODE_3;
    config.bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;

    ret_code_t err;
    if((err = nrf_drv_spi_init(&spi_master, &config, NULL, NULL)) != NRF_SUCCESS) {
        NRF_LOG_ERROR("Failing initializing SPI. Error %x", err);
        return false;
    }

    // Initialize the IO pins (CS, WP and HOLD)
    // CS
    nrf_gpio_cfg_output(CS_PIN);
    nrf_gpio_pin_set(CS_PIN);

    // WP
    nrf_gpio_cfg_output(WP_PIN);
    nrf_gpio_pin_set(WP_PIN);

    // HOLD
    nrf_gpio_cfg_output(HOLD_PIN);
    nrf_gpio_pin_set(HOLD_PIN);

    return true;
}

void flash_hal_set_cs(bool status)
{
    if(status) nrf_gpio_pin_set(CS_PIN);
    else nrf_gpio_pin_clear(CS_PIN);
}

void flash_hal_set_wp(bool status)
{
    if(status) nrf_gpio_pin_set(WP_PIN);
    else nrf_gpio_pin_clear(WP_PIN);
}

uint8_t flash_hal_exchange_byte(uint8_t byte)
{
    uint8_t rcv;
    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi_master, &byte, 1, &rcv, 1));
    return  rcv;
}

void flash_hal_write_bytes(uint8_t* data, unsigned int length)
{
    // For 255 and less bytes a single transfer is enough
    if(length < 256) {
        APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi_master, data, length, NULL, 0));
    }
    else {
        // We need to do multiple transfers for more than 255 bytes because the nrf_drv_spi_transfer()
        //  can only transfer up to 255 at once
        for(unsigned int n = 0; n < length;) {

            // Calculate the number of bytes to send in this chunk
            unsigned int transfer_size = length - n;
            if(transfer_size > 255) {
                transfer_size = 255;
            }

            APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi_master, data + n, transfer_size, NULL, 0));
            n += transfer_size;
        }
    }
}

void flash_hal_read_bytes(uint8_t* out, unsigned int length)
{
    // For 255 and less bytes a single transfer is enough
    if(length < 256) {
        APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi_master, NULL, 0, out, length));
    }
    else {
        // We need to do multiple transfers for more than 255 bytes because the nrf_drv_spi_transfer()
        //  can only transfer up to 255 at once
        for(unsigned int n = 0; n < length;) {

            // Calculate the number of bytes to send in this chunk
            unsigned int transfer_size = length - n;
            if(transfer_size > 255) {
                transfer_size = 255;
            }

            APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi_master, NULL, 0, out + n, transfer_size));
            n += transfer_size;
        }
    }
}