#include "cc1101.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "config/cansat.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "spi_manager/spi_manager.h"
#include "esp_timer.h"

#include "esp_log.h"

static const char* TAG = "cc1101";

#define WRITE_BURST         0x40
#define READ_SINGLE         0x80
#define READ_BURST          0xC0
#define BYTES_IN_RXFIFO     0x47
#define TIMEOUT_PINS        50      // In us
#define TIMEOUT_SPI         100     // In ms

static int channel = 1;
static int rx_bw = 0x08;
static int F2 = 16;
static int F1 = 176;
static int F0 = 113;
static int pa_level = PA10;

static const uint8_t PA_TABLE10[]    = {0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00};
static const uint8_t PA_TABLE7[]     = {0x00,0xC8,0x00,0x00,0x00,0x00,0x00,0x00};
static const uint8_t PA_TABLE5[]     = {0x00,0x84,0x00,0x00,0x00,0x00,0x00,0x00};
static const uint8_t PA_TABLE0[]     = {0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00};
static const uint8_t PA_TABLE_10[]   = {0x00,0x34,0x00,0x00,0x00,0x00,0x00,0x00};
static const uint8_t PA_TABLE_15[]   = {0x00,0x1D,0x00,0x00,0x00,0x00,0x00,0x00};
static const uint8_t PA_TABLE_20[]   = {0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00};
static const uint8_t PA_TABLE_30[]   = {0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00};

static struct spi_transaction_t transaction;
static uint8_t tx_buffer[CC1101_TX_BUFFER_SIZE];
static uint8_t rx_buffer[CC1101_RX_BUFFER_SIZE];

static inline bool wait_for_low(gpio_num_t pin, unsigned int timeout)
{
    // Wait for pin to get low
	while(gpio_get_level(pin))
    {
        int64_t start = esp_timer_get_time();
        if(esp_timer_get_time() - start > timeout)
        {
            return false;
        }
    }
    
    return true;
}

static void spi_write_reg(uint8_t addr, uint8_t value)
{
    gpio_set_level(CS_PIN, 0);
    wait_for_low(MISO_PIN, TIMEOUT_PINS);

    transaction.length = 2*8;
    transaction.rxlength = 0;   // Receive same byte quantity as length
    transaction.flags = 0x00000000;
    transaction.tx_buffer = tx_buffer;
    tx_buffer[0] = addr;
    tx_buffer[1] = value;

    spi_manager_device_transmit(&transaction, pdMS_TO_TICKS(TIMEOUT_SPI));
    gpio_set_level(CS_PIN, 1);
}

static void spi_send_cmd(uint8_t cmd)
{
    transaction.length = 1*8;
    transaction.rxlength = 0;   // Receive same byte quantity as length
    transaction.tx_buffer = tx_buffer;
    tx_buffer[0] = cmd;
    transaction.flags = 0x00000000;
    spi_manager_device_transmit(&transaction, pdMS_TO_TICKS(TIMEOUT_SPI));
}

static void spi_write_burst_reg(uint8_t addr, uint8_t *buffer, uint8_t num)
{
    gpio_set_level(CS_PIN, 0);
    wait_for_low(MISO_PIN, TIMEOUT_PINS);

    transaction.length = (num + 1)*8;
    transaction.rxlength = 0;   // Receive same byte quantity as length
    transaction.flags = 0x00000000;
    transaction.tx_buffer = tx_buffer;
    transaction.rx_buffer = rx_buffer;
    tx_buffer[0] = addr | WRITE_BURST;
    memcpy(&tx_buffer[1], buffer, num);

    // Send the register and all the data now
    spi_manager_device_transmit(&transaction, pdMS_TO_TICKS(TIMEOUT_SPI));
    gpio_set_level(CS_PIN, 1);
}

static uint8_t spi_read_reg(uint8_t addr)
{
    gpio_set_level(CS_PIN, 0);
    wait_for_low(MISO_PIN, TIMEOUT_PINS);

    transaction.length = 2*8;
    transaction.rxlength = 0;   // Receive same byte quantity as length
    transaction.flags = 0x00000000;
    transaction.tx_buffer = tx_buffer;
    transaction.rx_buffer = rx_buffer;
    tx_buffer[0] = addr | READ_SINGLE;
    tx_buffer[1] = 0x00;    // Dummy byte to request a read

    spi_manager_device_transmit(&transaction, pdMS_TO_TICKS(TIMEOUT_SPI));
    gpio_set_level(CS_PIN, 1);

    return rx_buffer[1];
}

static void spi_read_burst_reg(uint8_t addr, uint8_t *buffer, uint8_t num)
{
    gpio_set_level(CS_PIN, 0);
    wait_for_low(MISO_PIN, TIMEOUT_PINS);

    transaction.length = (num + 1)*8;
    transaction.rxlength = 0;   // Receive same byte quantity as length
    transaction.flags = 0x00000000;
    transaction.tx_buffer = tx_buffer;
    transaction.rx_buffer = buffer;
    tx_buffer[0] = addr | READ_BURST;

    // Send 0x00 for every byte we want to read
    memset(&tx_buffer[1], 0x00, num);

    spi_manager_device_transmit(&transaction, pdMS_TO_TICKS(TIMEOUT_SPI));
    gpio_set_level(CS_PIN, 1);
}

bool cc1101_reset(void)
{
    gpio_set_level(CS_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
	gpio_set_level(CS_PIN, 1);
	vTaskDelay(pdMS_TO_TICKS(10));
	gpio_set_level(CS_PIN, 0);

    // Wait for MISO pin to get low
    if(!wait_for_low(MISO_PIN, TIMEOUT_PINS))
    {
        return false;
    }

    // Reset the transceiver
    spi_send_cmd(CC1101_SRES);

    // Wait for reset
    if(!wait_for_low(MISO_PIN, TIMEOUT_PINS))
    {
        return false;
    }

	gpio_set_level(CS_PIN, 1);
    return true;
}

void cc1101_strobe_cmd(uint8_t strobe)
{
    gpio_set_level(CS_PIN, 0);
    // Wait for MISO pin to get low
    if(!wait_for_low(MISO_PIN, TIMEOUT_PINS))
    {
        ESP_LOGW(TAG, "Timeout MISO on cc1101_strobe_cmd()");
        return false;
    }

    spi_send_cmd(strobe);

    gpio_set_level(CS_PIN, 1);
}

bool cc1101_reg_config_settings(cc1101_pa_t pa)
{
    spi_write_reg(CC1101_FSCTRL1, 0x06);
    spi_write_reg(CC1101_FSCTRL0, 0x00);
    spi_write_reg(CC1101_FREQ2, F2);
    spi_write_reg(CC1101_FREQ1, F1);
    spi_write_reg(CC1101_FREQ0, F0);
      
    switch(pa)
    {
        case PA10:
            spi_write_burst_reg(CC1101_PATABLE, (uint8_t *)PA_TABLE10, sizeof(PA_TABLE10));
            break;
        case PA7:
            spi_write_burst_reg(CC1101_PATABLE, (uint8_t *)PA_TABLE7, sizeof(PA_TABLE7));
            break;
        case PA5:
            spi_write_burst_reg(CC1101_PATABLE, (uint8_t *)PA_TABLE5, sizeof(PA_TABLE5));
            break;        
        case PA0:
            spi_write_burst_reg(CC1101_PATABLE, (uint8_t *)PA_TABLE0, sizeof(PA_TABLE0));
            break;
        case PA_10:
            spi_write_burst_reg(CC1101_PATABLE, (uint8_t *)PA_TABLE_10, sizeof(PA_TABLE_10));
            break;
        case PA_15:
            spi_write_burst_reg(CC1101_PATABLE, (uint8_t *)PA_TABLE_15, sizeof(PA_TABLE_15));
            break;
        case PA_20:
            spi_write_burst_reg(CC1101_PATABLE, (uint8_t *)PA_TABLE_20, sizeof(PA_TABLE_20));
            break;
        case PA_30:
            spi_write_burst_reg(CC1101_PATABLE, (uint8_t *)PA_TABLE_30, sizeof(PA_TABLE_30));
            break;
	}
    pa_level = pa;
 
    spi_write_reg(CC1101_MDMCFG4,  rx_bw);  // DRATE_E = 8
    spi_write_reg(CC1101_MDMCFG3,  0x83);   // With DRATE_E on MDMCFG4 = 8 this gives 9600 bauds 
    spi_write_reg(CC1101_MDMCFG2,  0x13);   // 30/32 sync word, no Manchester encoding, GFSK modulation, DC filter before modulator
    spi_write_reg(CC1101_MDMCFG1,  0x00);   // 2 preamble bytes, no forward error correction
    spi_write_reg(CC1101_MDMCFG0,  0xF8);   // 200 kHz channel spacing together with CHANSPC_E bits in MDMCFG1
    spi_write_reg(CC1101_CHANNR,   channel);// Channel number
    spi_write_reg(CC1101_DEVIATN,  0x15);
    spi_write_reg(CC1101_FREND1,   0x56);
    spi_write_reg(CC1101_FREND0,   0x11);
    spi_write_reg(CC1101_MCSM0,    0x18);
    spi_write_reg(CC1101_FOCCFG,   0x16);
    spi_write_reg(CC1101_BSCFG,    0x1C);
    spi_write_reg(CC1101_AGCCTRL2, 0xC7);
  	spi_write_reg(CC1101_AGCCTRL1, 0x00);
    spi_write_reg(CC1101_AGCCTRL0, 0xB2);
    spi_write_reg(CC1101_FSCAL3,   0xE9);   // Value given by TI SmartRF Studio
  	spi_write_reg(CC1101_FSCAL2,   0x2A);   // Value given by TI SmartRF Studio
  	spi_write_reg(CC1101_FSCAL1,   0x00);   // Value given by TI SmartRF Studio
    spi_write_reg(CC1101_FSCAL0,   0x1F);   // Value given by TI SmartRF Studio
    spi_write_reg(CC1101_FSTEST,   0x59);   
    spi_write_reg(CC1101_TEST2,    0x81);   // Value given by TI SmartRF Studio
    spi_write_reg(CC1101_TEST1,    0x35);   // Value given by TI SmartRF Studio
    spi_write_reg(CC1101_TEST0,    0x09);   // Value given by TI SmartRF Studio
    spi_write_reg(CC1101_IOCFG2,   0x2E);   // Serial clock is synchronous to the data in synchronous serial mode
    spi_write_reg(CC1101_IOCFG0,   0x06);  	// Asserts GDO0 when sync word has been sent/received, and de-asserts at the end of the packet 
    spi_write_reg(CC1101_PKTCTRL1, 0x04);   // Two status bytes will be appended to the payload of the packet, including RSSI, LQI and CRC OK
											// No address check
    spi_write_reg(CC1101_PKTCTRL0, 0x05);	// Whitening OFF, CRC Enabled, variable length packets, packet length configured by the first byte after sync word
    spi_write_reg(CC1101_ADDR,     0x00);	// Address used for packet filtration (not used here)
    spi_write_reg(CC1101_PKTLEN,   0xFF); 	// 255 bytes max packet length allowed

    // Read some values to check that they were written
    uint8_t val;
    if((val = spi_read_reg(CC1101_PKTCTRL0)) != 0x05)
    {
        ESP_LOGE(TAG, "Error on CC1101_PKTCTRL0. Read: %d", val);
        return false;
    }
    if((val = spi_read_reg(CC1101_IOCFG0)) != 0x06)
    {
        ESP_LOGE(TAG, "Error on CC1101_IOCFG0. Read: %d", val);
        return false;
    }
    if((val = spi_read_reg(CC1101_MDMCFG4)) != rx_bw)
    {
        ESP_LOGE(TAG, "Error on CC1101_MDMCFG4. Read: %d", val);
        return false;
    }
    if((val = spi_read_reg(CC1101_FSCTRL1)) != 0x06)
    {
        ESP_LOGE(TAG, "Error on CC1101_FSCTRL1. Read: %d", val);
        return false;
    }

    return true;
}

bool cc1101_init(void)
{
    transaction.tx_buffer = tx_buffer;
    transaction.rx_buffer = rx_buffer;

    // Flush the TX and RX FIFOs
    cc1101_strobe_cmd(CC1101_SIDLE);
	cc1101_strobe_cmd(CC1101_SFTX);
    cc1101_strobe_cmd(CC1101_SFRX);

    if(!cc1101_reset())
    {
        ESP_LOGE(TAG, "Couldn't reset CC1101");
        return false;
    }
    return cc1101_reg_config_settings(pa_level);
}

void cc1101_set_rx(void)
{
    cc1101_strobe_cmd(CC1101_SRX);
}

void cc1101_set_tx(void)
{
    cc1101_strobe_cmd(CC1101_STX);
}

bool cc1101_set_mhz(float mhz)
{
    // Calculate F0, F1 and F1 to set the desired frequency
    float MHZ = mhz + 0.01;

    float freq2 = 26;
    float freq1 = 0.1015625;
    float freq0 = 0.00039675;

    float s1 = MHZ/freq2; 
    int s2 = s1;                             // freq2
    float s3 = s1-s2;
    float s4 = s3*100000000;
    float s5 = 255.0/100000000*s4;
    int s6 = s5;
    float s7 = (s5-s6);
    float s8 = s7*10;
    int s9;                                  // freq1
    if (s8>=5){ s9=s6+1; }
    if (s8<5){ s9=s6; }
    float s10 = MHZ - (freq2*s2+freq1*s9);
    float s11 = s10/freq0;
    int s12 = s11;
    float s13 = (s11-s12)*(10);
    int s14;                                 // freq0
    if (s13>=5){s14=s12+1;}
    if (s13<5){s14=s12;}

    F2 = s2;
    F1 = s9;
    F0 = s14;

    spi_write_reg(CC1101_FREQ2, F2);
    spi_write_reg(CC1101_FREQ1, F1);
    spi_write_reg(CC1101_FREQ0, F0);

    if(spi_read_reg(CC1101_FREQ0) != F0 || spi_read_reg(CC1101_FREQ1) != F1 || spi_read_reg(CC1101_FREQ2) != F2)
    {
        return false;
    }
    return true;
}

bool cc1101_send_data(uint8_t *tx_buffer, uint8_t size)
{
    if(size > 61)
    {
        ESP_LOGW(TAG, "Packet too large: %d", size);
        return false;
    }

    // Flush the TX FIFO (go into IDLE first)
    cc1101_strobe_cmd(CC1101_SIDLE);
	cc1101_strobe_cmd(CC1101_SFTX);
    ESP_LOGV(TAG, "TX FIFO before: %d", cc1101_bytes_in_tx_fifo());

    spi_write_reg(CC1101_TXFIFO, size);                     // Write packet length
    spi_write_burst_reg(CC1101_TXFIFO, tx_buffer, size);    // Write data
    ESP_LOGV(TAG, "TX FIFO after: %d", cc1101_bytes_in_tx_fifo());

	cc1101_set_tx();                                        // Enter TX mode to send the data

    // Internal CC1101 state machine status to check that TX mode has started
    ESP_LOGV(TAG, "CC1101 FSM: %d", cc1101_read_status(CC1101_MARCSTATE));
    
    // Wait for GDO0 to be set, indicates sync was transmitted 
    int64_t start = esp_timer_get_time();
    while(!cc1101_is_packet_sent_available())
    {
        if(esp_timer_get_time() - start > (TIMEOUT_SPI*1000))
        {
            // Failed to wait for sync
            ESP_LOGW(TAG, "Failed waiting sync");
            return false;
        }
    }

    // Wait for GDO0 to be cleared, indicates end of packet
    start = esp_timer_get_time();
    while(cc1101_is_packet_sent_available())
    {
        if(esp_timer_get_time() - start > (TIMEOUT_SPI*1000))
        {
            // Failed to wait for end of packet
            ESP_LOGW(TAG, "Failed waiting end of packet");
            return false;
        }
    }			

    return true;
}

void cc1101_set_rx_bw(uint8_t bw)
{
    switch (bw)
    {
        case 1:
            rx_bw = 0xF8;
            break;

        case 2:
            rx_bw = 0xE8;
            break;

        case 3:
            rx_bw = 0xD8;
            break;

        case 4:
            rx_bw = 0xC8;
            break;

        case 5:
            rx_bw = 0xB8;
            break;

        case 6:
            rx_bw = 0xA8;
            break;

        case 7:
            rx_bw = 0x98;
            break;

        case 8:
            rx_bw = 0x88;
            break;

        case 9:
            rx_bw = 0x78;
            break;

        case 10:
            rx_bw = 0x68;
            break;

        case 11:
            rx_bw = 0x58;
            break;

        case 12:
            rx_bw = 0x48;
            break;

        case 13:
            rx_bw = 0x38;
            break;

        case 14:
            rx_bw = 0x28;
            break;

        case 15:
            rx_bw = 0x18;
            break;

        case 16:
            rx_bw = 0x08;
            break;
    }
    spi_write_reg(CC1101_MDMCFG4, rx_bw);
}

void cc1101_set_channel(uint8_t chn)
{
    channel = chn;
    spi_write_reg(CC1101_CHANNR, channel);
}

int8_t cc1101_bytes_in_rx_fifo(void)
{
    // Read until we get the same number of bytes in the RX FIFO. This
    //  is needed as a workaround for an errate of the CC1101.
    // Read "SPI Read Synchronization Issue SPI read synchronization 
    //  results in incorrect read values for register fields that are continuously updated"
    //  at http://www.ti.com/lit/er/swrz020e/swrz020e.pdf
    unsigned v1, v2;
    for(unsigned int n = 0; n < 255; ++n)
    {
       v1 = cc1101_read_status(CC1101_RXBYTES);
       v2 = cc1101_read_status(CC1101_RXBYTES);
       if(v1 == v2)
       {
           return v1;
       }
    }
    
    // FAILED!
    return -1;
}

int8_t cc1101_bytes_in_tx_fifo(void)
{
    // Read until we get the same number of bytes in the TX FIFO. This
    //  is needed as a workaround for an errate of the CC1101.
    // Read "SPI Read Synchronization Issue SPI read synchronization 
    //  results in incorrect read values for register fields that are continuously updated"
    //  at http://www.ti.com/lit/er/swrz020e/swrz020e.pdf
    unsigned v1, v2;
    for(unsigned int n = 0; n < 255; ++n)
    {
       v1 = cc1101_read_status(CC1101_TXBYTES);
       v2 = cc1101_read_status(CC1101_TXBYTES);
       if(v1 == v2)
       {
           return v1;
       }
    }
    
    // FAILED!
    return -1;
}

bool cc1101_read_data(cc1101_packet_t* packet)
{
    // Read the number of bytes in the RX FIFO
    unsigned int rx_bytes = cc1101_bytes_in_rx_fifo();

    // Any byte waiting to be read and no overflow?
    if (rx_bytes & 0x7F && !(rx_bytes & 0x80))
    {
        // Read data length. The first byte in the FIFO is the length.
        packet->length = spi_read_reg(CC1101_RXFIFO);

        // If packet is too long
        if (packet->length > CC1101_MAX_PACKET_SIZE || packet->length == 0)
        {
            ESP_LOGW(TAG, "Length error: %d", packet->length);
            packet->length = 0;
            return false;
        }
        else
        {
            uint8_t status[2];

            // Read data packet
            spi_read_burst_reg(CC1101_RXFIFO, packet->data, packet->length);
            // Read RSSI, LQI and CRC_OK
            spi_read_burst_reg(CC1101_RXFIFO, status, 2);

            packet->rssi = status[0];
            packet->lqi = status[1] & 0x7F;
            packet->crc_ok = status[1] & 0x80;

            return true;
        }
    }
    else
    {
        packet->length = 0;
        return false;
    }
}

uint8_t cc1101_read_status(uint8_t addr)
{
    gpio_set_level(CS_PIN, 0);
    wait_for_low(MISO_PIN, TIMEOUT_PINS);

    transaction.length = 2*8;
    transaction.rxlength = 0;   // Receive same byte quantity as length
    transaction.flags = 0x00000000;
    transaction.tx_buffer = tx_buffer;
    transaction.rx_buffer = rx_buffer;
    tx_buffer[0] = addr | READ_BURST;
    tx_buffer[1] = 0x00;    // Dummy byte to request a read

    spi_manager_device_transmit(&transaction, pdMS_TO_TICKS(TIMEOUT_SPI));
    gpio_set_level(CS_PIN, 1);

    return rx_buffer[1];
}

bool cc1101_is_packet_sent_available(void)
{
    uint8_t status = cc1101_read_status(CC1101_PKTSTATUS);

    // If GDO0 is set a packet was sent or received
    if(status & 0x01)
    {
        return true;
    }

    return false;
}