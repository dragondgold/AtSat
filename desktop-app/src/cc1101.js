function cc1101_driver()
{
    //***************************************CC1101 DEFINES**************************************************//
    // CC1101 CONFIG register
    const CC1101_IOCFG2       = 0x00;        // GDO2 output pin configuration
    const CC1101_IOCFG1       = 0x01;        // GDO1 output pin configuration
    const CC1101_IOCFG0       = 0x02;        // GDO0 output pin configuration
    const CC1101_FIFOTHR      = 0x03;        // RX FIFO and TX FIFO thresholds
    const CC1101_SYNC1        = 0x04;        // Sync word, high INT8U
    const CC1101_SYNC0        = 0x05;        // Sync word, low INT8U
    const CC1101_PKTLEN       = 0x06;        // Packet length
    const CC1101_PKTCTRL1     = 0x07;        // Packet automation control
    const CC1101_PKTCTRL0     = 0x08;        // Packet automation control
    const CC1101_ADDR         = 0x09;        // Device address
    const CC1101_CHANNR       = 0x0A;        // Channel number
    const CC1101_FSCTRL1      = 0x0B;        // Frequency synthesizer control
    const CC1101_FSCTRL0      = 0x0C;        // Frequency synthesizer control
    const CC1101_FREQ2        = 0x0D;        // Frequency control word, high INT8U
    const CC1101_FREQ1        = 0x0E;        // Frequency control word, middle INT8U
    const CC1101_FREQ0        = 0x0F;        // Frequency control word, low INT8U
    const CC1101_MDMCFG4      = 0x10;        // Modem configuration
    const CC1101_MDMCFG3      = 0x11;        // Modem configuration
    const CC1101_MDMCFG2      = 0x12;        // Modem configuration
    const CC1101_MDMCFG1      = 0x13;        // Modem configuration
    const CC1101_MDMCFG0      = 0x14;        // Modem configuration
    const CC1101_DEVIATN      = 0x15;        // Modem deviation setting
    const CC1101_MCSM2        = 0x16;        // Main Radio Control State Machine configuration
    const CC1101_MCSM1        = 0x17;        // Main Radio Control State Machine configuration
    const CC1101_MCSM0        = 0x18;        // Main Radio Control State Machine configuration
    const CC1101_FOCCFG       = 0x19;        // Frequency Offset Compensation configuration
    const CC1101_BSCFG        = 0x1A;        // Bit Synchronization configuration
    const CC1101_AGCCTRL2     = 0x1B;        // AGC control
    const CC1101_AGCCTRL1     = 0x1C;        // AGC control
    const CC1101_AGCCTRL0     = 0x1D;        // AGC control
    const CC1101_WOREVT1      = 0x1E;        // High INT8U Event 0 timeout
    const CC1101_WOREVT0      = 0x1F;        // Low INT8U Event 0 timeout
    const CC1101_WORCTRL      = 0x20;        // Wake On Radio control
    const CC1101_FREND1       = 0x21;        // Front end RX configuration
    const CC1101_FREND0       = 0x22;        // Front end TX configuration
    const CC1101_FSCAL3       = 0x23;        // Frequency synthesizer calibration
    const CC1101_FSCAL2       = 0x24;        // Frequency synthesizer calibration
    const CC1101_FSCAL1       = 0x25;        // Frequency synthesizer calibration
    const CC1101_FSCAL0       = 0x26;        // Frequency synthesizer calibration
    const CC1101_RCCTRL1      = 0x27;        // RC oscillator configuration
    const CC1101_RCCTRL0      = 0x28;        // RC oscillator configuration
    const CC1101_FSTEST       = 0x29;        // Frequency synthesizer calibration control
    const CC1101_PTEST        = 0x2A;        // Production test
    const CC1101_AGCTEST      = 0x2B;        // AGC test
    const CC1101_TEST2        = 0x2C;        // Various test settings
    const CC1101_TEST1        = 0x2D;        // Various test settings
    const CC1101_TEST0        = 0x2E;        // Various test settings

    // CC1101 strobe commands
    const CC1101_SRES         = 0x30;        // Reset chip.
    const CC1101_SFSTXON      = 0x31;        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                                             // If in RX/TX: Go to a wait state where only the synthesizer is
                                             // running (for quick RX / TX turnaround).
    const CC1101_SXOFF        = 0x32;        // Turn off crystal oscillator.
    const CC1101_SCAL         = 0x33;        // Calibrate frequency synthesizer and turn it off
                                             // (enables quick start).
    const CC1101_SRX          = 0x34;        // Enable RX. Perform calibration first if coming from IDLE and
                                             // MCSM0.FS_AUTOCAL=1.
    const CC1101_STX          = 0x35;        // In IDLE state: Enable TX. Perform calibration first if
                                             // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                             // Only go to TX if channel is clear.
    const CC1101_SIDLE        = 0x36;        // Exit RX / TX, turn off frequency synthesizer and exit
                                             // Wake-On-Radio mode if applicable.
    const CC1101_SAFC         = 0x37;        // Perform AFC adjustment of the frequency synthesizer
    const CC1101_SWOR         = 0x38;        // Start automatic RX polling sequence (Wake-on-Radio)
    const CC1101_SPWD         = 0x39;        // Enter power down mode when CSn goes high.
    const CC1101_SFRX         = 0x3A;        // Flush the RX FIFO buffer.
    const CC1101_SFTX         = 0x3B;        // Flush the TX FIFO buffer.
    const CC1101_SWORRST      = 0x3C;        // Reset real time clock.
    const CC1101_SNOP         = 0x3D;        // No operation. May be used to pad strobe commands to two
                                             // INT8Us for simpler software.
    // CC1101 STATUS register
    const CC1101_PARTNUM      = 0x30;
    const CC1101_VERSION      = 0x31;
    const CC1101_FREQEST      = 0x32;
    const CC1101_LQI          = 0x33;
    const CC1101_RSSI         = 0x34;
    const CC1101_MARCSTATE    = 0x35;
    const CC1101_WORTIME1     = 0x36;
    const CC1101_WORTIME0     = 0x37;
    const CC1101_PKTSTATUS    = 0x38;
    const CC1101_VCO_VC_DAC   = 0x39;
    const CC1101_TXBYTES      = 0x3A;
    const CC1101_RXBYTES      = 0x3B;

    // CC1101 PATABLE, TXFIFO, RXFIFO
    const CC1101_PATABLE      = 0x3E;
    const CC1101_TXFIFO       = 0x3F;
    const CC1101_RXFIFO       = 0x3F;

    const CC1101_TX_BUFFER_SIZE = 64;
    const CC1101_RX_BUFFER_SIZE = 64;

    const WRITE_BURST     = 0x40;
    const READ_SINGLE     = 0x80;
    const READ_BURST      = 0xC0;
    const BYTES_IN_RXFIFO = 0x47;
    const TIMEOUT_PINS    = 50;         // In us
    const TIMEOUT_SPI     = 100;        // In ms

    // The maximum supported data packet by this driver is 64 bytes. In order to receive/send more bytes
    //  than the size of the FIFO of the CC1101 some rework is needed.
    const CC1101_MAX_PACKET_SIZE = (64-3);

    let channel = 1;
    let rx_bw = 0x08;
    let F2 = 16;
    let F1 = 176;
    let F0 = 113;
    let pa_level = PA10;

    const PA_TABLE10  = [0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00];
    const PA_TABLE7   = [0x00,0xC8,0x00,0x00,0x00,0x00,0x00,0x00];
    const PA_TABLE5   = [0x00,0x84,0x00,0x00,0x00,0x00,0x00,0x00];
    const PA_TABLE0   = [0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00];
    const PA_TABLE_10 = [0x00,0x34,0x00,0x00,0x00,0x00,0x00,0x00];
    const PA_TABLE_15 = [0x00,0x1D,0x00,0x00,0x00,0x00,0x00,0x00];
    const PA_TABLE_20 = [0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00];
    const PA_TABLE_30 = [0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00];

    /**
     * Wait for a pin from the SPI interface to get low
     * @param {*} pin Pin to use
     * @param {*} timeout Timeout
     */
    function wait_for_low(pin, timeout)
    {
        // TODO: implement
        /*
        // Wait for pin to get low
        while(gpio_get_level(pin))
        {
            let start = esp_timer_get_time();
            if(esp_timer_get_time() - start > timeout)
            {
                return false;
            }
        }
        
        return true;*/
    }

    /**
     * Write a register
     * @param {*} addr Address of the register
     * @param {*} value Value to write in the register
     */
    function spi_write_reg(addr, value)
    {
        // TODO: implement
        /*
        gpio_set_level(CS_PIN, 0);
        wait_for_low(MISO_PIN, TIMEOUT_PINS);

        transaction.length = 2*8;
        transaction.rxlength = 0;   // Receive same byte quantity as length
        transaction.flags = 0x00000000;
        transaction.tx_buffer = tx_buffer;
        tx_buffer[0] = addr;
        tx_buffer[1] = value;

        spi_manager_device_transmit(&transaction, pdMS_TO_TICKS(TIMEOUT_SPI));
        gpio_set_level(CS_PIN, 1);*/
    }

    /**
     * Send a command
     * @param {*} cmd command to send
     */
    function spi_send_cmd(cmd)
    {
        // TODO: implement
        /*
        transaction.length = 1*8;
        transaction.rxlength = 0;   // Receive same byte quantity as length
        transaction.tx_buffer = tx_buffer;
        tx_buffer[0] = cmd;
        transaction.flags = 0x00000000;
        spi_manager_device_transmit(&transaction, pdMS_TO_TICKS(TIMEOUT_SPI));*/
    }

    /**
     * Write a register in burst mode
     * @param {*} addr Address of the register
     * @param {*} buffer Array of bytes to send
     */
    function spi_write_burst_reg(addr, buffer)
    {
        // TODO: implement
        /*
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
        gpio_set_level(CS_PIN, 1);*/
    }

    /**
     * Read a register
     * @param {*} addr Address of the register
     * @returns byte read
     */
    function spi_read_reg(addr)
    {
        // TODO: implement
        /*
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

        return rx_buffer[1];*/
    }

    /**
     * Read a register in burst mode
     * @param {*} addr Address of the register
     * @param {*} length Number of bytes to read
     * @returns array of bytes read
     */
    function spi_read_burst_reg(addr, length)
    {
        // TODO: implement
        /*
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
        gpio_set_level(CS_PIN, 1);*/
    }

    /**
     * Reset the CC1101.
     * @returns true if reset was successful
     */
    function cc1101_reset()
    {
        // TODO: implement
        /*
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
        return true;*/
    }

    /**
     * Send a strobe command
     * @param {*} strobe strobe command to send
     */
    function cc1101_strobe_cmd(strobe)
    {
        // TODO: implement
        /*
        gpio_set_level(CS_PIN, 0);
        // Wait for MISO pin to get low
        if(!wait_for_low(MISO_PIN, TIMEOUT_PINS))
        {
            ESP_LOGW(TAG, "Timeout MISO on cc1101_strobe_cmd()");
            return false;
        }

        spi_send_cmd(strobe);

        gpio_set_level(CS_PIN, 1);*/
    }

    /**
     * Set the default configuration for this driver
     * @param {*} pa PA level. The valid values are 10, 7, 5, 0, -10, -15, -20 and -30
     * @returns true if configuration was successful
     */
    function cc1101_reg_config_settings(pa)
    {
        spi_write_reg(CC1101_FSCTRL1, 0x06);
        spi_write_reg(CC1101_FSCTRL0, 0x00);
        spi_write_reg(CC1101_FREQ2, F2);
        spi_write_reg(CC1101_FREQ1, F1);
        spi_write_reg(CC1101_FREQ0, F0);
        
        switch(pa)
        {
            case 10:
                spi_write_burst_reg(CC1101_PATABLE, PA_TABLE10);
                break;
            case 7:
                spi_write_burst_reg(CC1101_PATABLE, PA_TABLE7);
                break;
            case 5:
                spi_write_burst_reg(CC1101_PATABLE, PA_TABLE5);
                break;        
            case 0:
                spi_write_burst_reg(CC1101_PATABLE, PA_TABLE0);
                break;
            case -10:
                spi_write_burst_reg(CC1101_PATABLE, PA_TABLE_10);
                break;
            case -15:
                spi_write_burst_reg(CC1101_PATABLE, PA_TABLE_15);
                break;
            case -20:
                spi_write_burst_reg(CC1101_PATABLE, PA_TABLE_20);
                break;
            case -30:
                spi_write_burst_reg(CC1101_PATABLE, PA_TABLE_30);
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
        let val = 0;
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

    /**
     * Init the CC1101 transceiver
     * @returns true if init was successful
     */
    function cc1101_init()
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

    /**
     * Set RX mode
     * @param {*} clear true to clear the RX FIFO
     */
    function cc1101_set_rx(clear)
    {
        if(clear)
        {
            cc1101_strobe_cmd(CC1101_SIDLE);
            cc1101_strobe_cmd(CC1101_SFRX);
        }
        cc1101_strobe_cmd(CC1101_SRX);
    }

    /**
     * Set TX mode
     */
    function cc1101_set_tx()
    {
        cc1101_strobe_cmd(CC1101_STX);
    }

    /**
     * Set carrier frequency
     * @param {*} mhz frequency in MHz
     * @returns true if successful
     */
    function cc1101_set_mhz(mhz)
    {
        // Calculate F0, F1 and F1 to set the desired frequency
        let MHZ = mhz + 0.01;

        let freq2 = 26;
        let freq1 = 0.1015625;
        let freq0 = 0.00039675;

        let s1 = MHZ/freq2; 
        let s2 = s1;                             // freq2
        let s3 = s1-s2;
        let s4 = s3*100000000;
        let s5 = 255.0/100000000*s4;
        let s6 = s5;
        let s7 = (s5-s6);
        let s8 = s7*10;
        let s9;                                  // freq1
        if (s8>=5){ s9=s6+1; }
        if (s8<5){ s9=s6; }
        let s10 = MHZ - (freq2*s2+freq1*s9);
        let s11 = s10/freq0;
        let s12 = s11;
        let s13 = (s11-s12)*(10);
        let s14;                                 // freq0
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

    /**
     * Send data
     * @param {*} tx_buffer array of bytes to send
     */
    function cc1101_send_data(tx_buffer)
    {
        if(tx_buffer.length > 61)
        {
            ESP_LOGW(TAG, "Packet too large: %d", tx_buffer.length);
            return false;
        }

        // Flush the TX FIFO (go into IDLE first)
        cc1101_strobe_cmd(CC1101_SIDLE);
        cc1101_strobe_cmd(CC1101_SFTX);
        ESP_LOGV(TAG, "TX FIFO before: %d", cc1101_bytes_in_tx_fifo());

        spi_write_reg(CC1101_TXFIFO, size);                     // Write packet length
        spi_write_burst_reg(CC1101_TXFIFO, tx_buffer);          // Write data
        ESP_LOGV(TAG, "TX FIFO after: %d", cc1101_bytes_in_tx_fifo());

        cc1101_set_tx();                                        // Enter TX mode to send the data

        // Internal CC1101 state machine status to check that TX mode has started
        ESP_LOGV(TAG, "CC1101 FSM: %d", cc1101_read_status(CC1101_MARCSTATE));
        
        // TODO: implement timeout mechanism
        // Wait for GDO0 to be set, indicates sync was transmitted 
        let start = esp_timer_get_time();
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

    /**
     * Set receiving bandwidth
     * @param {*} bw bandwidth level from 1 to 16
     */
    function cc1101_set_rx_bw(bw)
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

    /**
     * Set operating channel
     * @param {*} chn channel
     */
    function cc1101_set_channel(chn)
    {
        channel = chn;
        spi_write_reg(CC1101_CHANNR, channel);
    }

    /**
     * Checks the bytes available in the RX FIFO
     * @returns bytes available in the RX FIFO
     */
    function cc1101_bytes_in_rx_fifo()
    {
        // Read until we get the same number of bytes in the RX FIFO. This
        //  is needed as a workaround for an errate of the CC1101.
        // Read "SPI Read Synchronization Issue SPI read synchronization 
        //  results in incorrect read values for register fields that are continuously updated"
        //  at http://www.ti.com/lit/er/swrz020e/swrz020e.pdf
        let v1 = 0, v2 = 0;
        for(let n = 0; n < 255; ++n)
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

    /**
     * Checks the bytes available in the TX FIFO
     * @returns bytes available in the TX FIFO
     */
    function cc1101_bytes_in_tx_fifo()
    {
        // Read until we get the same number of bytes in the TX FIFO. This
        //  is needed as a workaround for an errate of the CC1101.
        // Read "SPI Read Synchronization Issue SPI read synchronization 
        //  results in incorrect read values for register fields that are continuously updated"
        //  at http://www.ti.com/lit/er/swrz020e/swrz020e.pdf
        let v1 = 0, v2 = 0;
        for(let n = 0; n < 255; ++n)
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

    /**
     * Read data from the RX FIFO
     * @returns packet read
     */
    function cc1101_read_data()
    {
        // Create packet here
        let packet = 
        {
            length: 0,
            data: [],
            rssi: 0,
            lqi: 0,
            crc_ok: false,
            valid: false,
        }

        // Read the number of bytes in the RX FIFO
        let rx_bytes = cc1101_bytes_in_rx_fifo();

        // Any byte waiting to be read and no overflow?
        if (rx_bytes & 0x7F && !(rx_bytes & 0x80))
        {
            // Read data length. The first byte in the FIFO is the length.
            packet.length = spi_read_reg(CC1101_RXFIFO);

            // If packet is too long
            if (packet.length > CC1101_MAX_PACKET_SIZE || packet.length == 0)
            {
                ESP_LOGW(TAG, "Length error: %d", packet.length);
                packet.length = 0;
                return packet;
            }
            else
            {
                let status = [];

                // Read data packet
                packet.data = spi_read_burst_reg(CC1101_RXFIFO, packet.length);
                // Read RSSI, LQI and CRC_OK
                status = spi_read_burst_reg(CC1101_RXFIFO, status, 2);

                packet.rssi = status[0];
                packet.lqi = status[1] & 0x7F;
                packet.crc_ok = status[1] & 0x80;
                packet.valid = true;

                return packet;
            }
        }
        else
        {
            return packet;
        }
    }

    /**
     * Read status register
     * @param {*} addr address of register
     * @returns status register
     */
    function cc1101_read_status(addr)
    {
        /*
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

        return rx_buffer[1];*/
    }

    /**
     * Check if a packet was sent (when sending packet) or received
     *  (when expecting a packet).
     * @returns true packet was sent or a packet was received
     */
    function cc1101_is_packet_sent_available()
    {
        let status = cc1101_read_status(CC1101_PKTSTATUS);

        // If GDO0 is set a packet was sent or received
        if(status & 0x01)
        {
            return true;
        }

        return false;
    }

    return
    {
        cc1101_init: cc1101_init,
        cc1101_strobe_cmd: cc1101_strobe_cmd,
        cc1101_reg_config_settings: cc1101_reg_config_settings,
        cc1101_set_rx: cc1101_set_rx,
        cc1101_set_tx: cc1101_set_tx,
        cc1101_set_mhz: cc1101_set_mhz,
        cc1101_send_data: cc1101_send_data,
        cc1101_set_rx_bw: cc1101_set_rx_bw,
        cc1101_set_channel: cc1101_set_channel,
        cc1101_bytes_in_rx_fifo: cc1101_bytes_in_rx_fifo,
        cc1101_bytes_in_tx_fifo: cc1101_bytes_in_tx_fifo,
        cc1101_read_data: cc1101_read_data,
        cc1101_read_status: cc1101_read_status,
        cc1101_is_packet_sent_available: cc1101_is_packet_sent_available
    }
}