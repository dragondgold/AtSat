const execSync = require('child_process').execSync;

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

const WRITE_BURST     = 0x40;
const READ_SINGLE     = 0x80;
const READ_BURST      = 0xC0;
const BYTES_IN_RXFIFO = 0x47;
const TIMEOUT_SPI     = 50;         // In ms

// The maximum supported data packet by this driver is 64 bytes. In order to receive/send more bytes
//  than the size of the FIFO of the CC1101 some rework is needed.
const CC1101_MAX_PACKET_SIZE = (64-3);
const CS_WAIT_TIME = 2;             // Wait 2 ms when setting CS low

let MCP2210CLI_PATH = "";
let channel = 1;
let rx_bw = 0x08;
let F2 = 16;
let F1 = 176;
let F0 = 113;
let pa_level = 10;

const PA_TABLE10  = [0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00];
const PA_TABLE7   = [0x00,0xC8,0x00,0x00,0x00,0x00,0x00,0x00];
const PA_TABLE5   = [0x00,0x84,0x00,0x00,0x00,0x00,0x00,0x00];
const PA_TABLE0   = [0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00];
const PA_TABLE_10 = [0x00,0x34,0x00,0x00,0x00,0x00,0x00,0x00];
const PA_TABLE_15 = [0x00,0x1D,0x00,0x00,0x00,0x00,0x00,0x00];
const PA_TABLE_20 = [0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00];
const PA_TABLE_30 = [0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00];

const TAG = "CC1101";

module.exports =
{
    /**
     * Set CS pin level
     * @param {*} level true or false
     */
    set_cs_level: function(level)
    {
        // Set CS to 1 or 0
        if(level)
        {
            let cmd_string = " -gpioW=gp0high";
            let output = execSync(MCP2210CLI_PATH + cmd_string);
        }
        else
        {
            // Set to 0
            let cmd_string = " -gpioW=gp0low";
            let output = execSync(MCP2210CLI_PATH + cmd_string);
        }
    },

    /**
     * Transfer data to the SPI
     * @param {Array} data data to send
     * @returns {Object} object with the parameters .data that contains the read data and
     *  .error that is true if an error ocurred.
     */
    mcp2210_transfer_data: function(data)
    {
        let data_string = "";
        for(let n = 0; n < data.length; ++n)
        {
            data_string += data[n].toString(16) + ",";
        }
        data_string = data_string.substring(0, data_string.lastIndexOf(","));
        //console.log("data_string: " + data_string);

        let cmd_string = " -spitxfer=" + data_string + " -bd=4000000 -cs=gp0 -idle=ffff -actv=0000 -csdly=1";
        //console.log("Data: " + data);
        //console.log("CMD:" + cmd_string);
        let output = execSync(MCP2210CLI_PATH + cmd_string);

        //console.log(output.toString());
        let lines = output.toString().split(">");

        let obj = 
        {
            error: true,
            data: []
        }

        // Invalid response
        if(lines.length == 0)
        {
            return obj;
        }
        //console.log("Lines: " + lines);

        for(let n = 0; n < lines.length; ++n)
        {
            // This line contains the RxData?
            if(lines[n].indexOf("RxData:") >= 0)
            {
                // Get the chunk of the string where the received bytes are
                let data_lines = lines[n].substring(lines[n].indexOf(" ") + 1).split(",");

                // Invalid
                if(data_lines.length == 0)
                {
                    return obj;
                }

                for(let line = 0; line < data_lines.length; ++line)
                {
                    //console.log("Data line: " + data_lines[line]);
                    obj.data.push(parseInt(data_lines[line], 16));
                }

                obj.error = false;
                return obj;
            }
            else
            {
                //console.error("Couldn't find RxData:");
            }
        }

        return obj;
    },

    /**
     * Wait for a to return the passed value
     * @param {Function} func from which to evaluate the return value
     * @param {*} value value that must be satisfied
     * @param {*} timeout timeout in ms
     * @returns {Promise} promise that is resolve or rejected
     */
    wait_for_condition: function(func, value, timeout)
    {
        return new Promise(function(resolve, reject)
        {
            let count = 0;

            // Execute every 1 ms
            let interval = setInterval(function()
            {
                ++count;
                // Wait for to return the desired value
                if(func() === value)
                {
                    clearInterval(interval);
                    resolve();
                    return;
                }
                else if(count > timeout)
                {
                    clearInterval(interval);
                    reject();
                    return;
                }
            }, 1);
        });
    },

    /**
     * Write a register
     * @param {*} addr Address of the register
     * @param {*} value Value to write in the register
     * @returns read-back value
     */
    spi_write_reg: function(addr, value)
    {
        let obj = this.mcp2210_transfer_data([addr, value]);

        if(!obj.error)
        {
            return obj.data[1];
        }
        
        return [0, 0];
    },

    /**
     * Send a command
     * @param {*} cmd command to send
     */
    spi_send_cmd: function(cmd)
    {
        this.mcp2210_transfer_data([cmd]);
    },

    /**
     * Write a register in burst mode
     * @param {*} addr Address of the register
     * @param {*} buffer Array of bytes to send
     */
    spi_write_burst_reg: function(addr, buffer)
    {
        var obj = this.mcp2210_transfer_data([addr | WRITE_BURST].concat(buffer));

        if(!obj.error)
        {
            return true;
        }
        else
        {
            return false;
        }
    },

    /**
     * Read a register
     * @param {*} addr Address of the register
     * @returns byte read
     */
    spi_read_reg: function(addr)
    {
        let obj = this.mcp2210_transfer_data([addr | READ_SINGLE, 0x00]);
        if(!obj.error)
        {
            return obj.data[1];
        }

        // ERROR!
        return 0;
    },

    /**
     * Read a register in burst mode
     * @param {*} addr Address of the register
     * @param {*} length Number of bytes to read
     * @returns array of bytes read
     */
    spi_read_burst_reg: function(addr, length)
    {
        let data = [addr | READ_BURST];
        for(let n = 0; n < length; ++n)
        {
            // Add dummy bytes to read
            data.push(0x00);
        }

        let obj = this.mcp2210_transfer_data([data]);

        if(!obj.error)
        {
            return obj.data;
        }
        else
        {
            return [];
        }
    },

    /**
     * Reset the CC1101.
     * @returns true if reset was successful
     */
    cc1101_reset: function()
    {
        // This will block the thread but that's ok, this code should run
        //  on a worker thread
        function sleep(delay) {
            var start = new Date().getTime();
            while (new Date().getTime() < start + delay);
        }

        this.set_cs_level(false);
        sleep(10);
        this.set_cs_level(true);
        sleep(10);
        this.set_cs_level(false);
        sleep(10);

        // Reset the transceiver
        this.spi_send_cmd(CC1101_SRES);

        // Wait for reset
        sleep(10);

        this.set_cs_level(true);
        return true;
    },

    /**
     * Send a strobe command
     * @param {*} strobe strobe command to send
     */
    cc1101_strobe_cmd: function(strobe)
    {
        this.set_cs_level(false);
        this.spi_send_cmd(strobe);
        this.set_cs_level(true);
    },

    /**
     * Set the default configuration for this driver
     * @param {*} pa PA level. The valid values are 10, 7, 5, 0, -10, -15, -20 and -30
     * @returns true if configuration was successful
     */
    cc1101_reg_config_settings: function(pa)
    {
        this.spi_write_reg(CC1101_FSCTRL1, 0x06);
        this.spi_write_reg(CC1101_FSCTRL0, 0x00);
        this.spi_write_reg(CC1101_FREQ2, F2);
        this.spi_write_reg(CC1101_FREQ1, F1);
        this.spi_write_reg(CC1101_FREQ0, F0);
        
        switch(pa)
        {
            case 10:
                this.spi_write_burst_reg(CC1101_PATABLE, PA_TABLE10);
                break;
            case 7:
                this.spi_write_burst_reg(CC1101_PATABLE, PA_TABLE7);
                break;
            case 5:
                this.spi_write_burst_reg(CC1101_PATABLE, PA_TABLE5);
                break;        
            case 0:
                this.spi_write_burst_reg(CC1101_PATABLE, PA_TABLE0);
                break;
            case -10:
                this.spi_write_burst_reg(CC1101_PATABLE, PA_TABLE_10);
                break;
            case -15:
                this.spi_write_burst_reg(CC1101_PATABLE, PA_TABLE_15);
                break;
            case -20:
                this.spi_write_burst_reg(CC1101_PATABLE, PA_TABLE_20);
                break;
            case -30:
                this.spi_write_burst_reg(CC1101_PATABLE, PA_TABLE_30);
                break;
        }
        pa_level = pa;
    
        this.spi_write_reg(CC1101_MDMCFG4,  rx_bw);  // DRATE_E = 8
        this.spi_write_reg(CC1101_MDMCFG3,  0x83);   // With DRATE_E on MDMCFG4 = 8 this gives 9600 bauds 
        this.spi_write_reg(CC1101_MDMCFG2,  0x13);   // 30/32 sync word, no Manchester encoding, GFSK modulation, DC filter before modulator
        this.spi_write_reg(CC1101_MDMCFG1,  0x00);   // 2 preamble bytes, no forward error correction
        this.spi_write_reg(CC1101_MDMCFG0,  0xF8);   // 200 kHz channel spacing together with CHANSPC_E bits in MDMCFG1
        this.spi_write_reg(CC1101_CHANNR,   channel);// Channel number
        this.spi_write_reg(CC1101_DEVIATN,  0x15);
        this.spi_write_reg(CC1101_FREND1,   0x56);
        this.spi_write_reg(CC1101_FREND0,   0x11);
        this.spi_write_reg(CC1101_MCSM0,    0x18);
        this.spi_write_reg(CC1101_FOCCFG,   0x16);
        this.spi_write_reg(CC1101_BSCFG,    0x1C);
        this.spi_write_reg(CC1101_AGCCTRL2, 0xC7);
        this.spi_write_reg(CC1101_AGCCTRL1, 0x00);
        this.spi_write_reg(CC1101_AGCCTRL0, 0xB2);
        this.spi_write_reg(CC1101_FSCAL3,   0xE9);      // Value given by TI SmartRF Studio
        this.spi_write_reg(CC1101_FSCAL2,   0x2A);      // Value given by TI SmartRF Studio
        this.spi_write_reg(CC1101_FSCAL1,   0x00);      // Value given by TI SmartRF Studio
        this.spi_write_reg(CC1101_FSCAL0,   0x1F);      // Value given by TI SmartRF Studio
        this.spi_write_reg(CC1101_FSTEST,   0x59);   
        this.spi_write_reg(CC1101_TEST2,    0x81);      // Value given by TI SmartRF Studio
        this.spi_write_reg(CC1101_TEST1,    0x35);      // Value given by TI SmartRF Studio
        this.spi_write_reg(CC1101_TEST0,    0x09);      // Value given by TI SmartRF Studio
        this.spi_write_reg(CC1101_IOCFG2,   0x2E);      // Serial clock is synchronous to the data in synchronous serial mode
        this.spi_write_reg(CC1101_IOCFG0,   0x06);      // Asserts GDO0 when sync word has been sent/received, and de-asserts at the end of the packet 
        this.spi_write_reg(CC1101_PKTCTRL1, 0x04);      // Two status bytes will be appended to the payload of the packet, including RSSI, LQI and CRC OK
                                                        // No address check
        this.spi_write_reg(CC1101_PKTCTRL0, 0x05);	    // Whitening OFF, CRC Enabled, variable length packets, packet length configured by the first byte after sync word
        this.spi_write_reg(CC1101_ADDR,     0x00);	    // Address used for packet filtration (not used here)
        this.spi_write_reg(CC1101_PKTLEN,   0xFF); 	    // 255 bytes max packet length allowed

        // Read some values to check that they were written
        let val = 0;
        if((val = this.spi_read_reg(CC1101_PKTCTRL0)) != 0x05)
        {
            console.error(TAG + ":" + "Error on CC1101_PKTCTRL0. Read: %d", val);
            return false;
        }
        if((val = this.spi_read_reg(CC1101_IOCFG0)) != 0x06)
        {
            console.error(TAG + ":" + "Error on CC1101_IOCFG0. Read: %d", val);
            return false;
        }
        if((val = this.spi_read_reg(CC1101_MDMCFG4)) != rx_bw)
        {
            console.error(TAG + ":" + "Error on CC1101_MDMCFG4. Read: %d", val);
            return false;
        }
        if((val = this.spi_read_reg(CC1101_FSCTRL1)) != 0x06)
        {
            console.error(TAG + ":" + "Error on CC1101_FSCTRL1. Read: %d", val);
            return false;
        }

        return true;
    },

    /**
     * Init the CC1101 transceiver
     * @param path path to the MCP2210CLI.exe executable
     * @returns true if init was successful
     */
    cc1101_init: function(path)
    {
        MCP2210CLI_PATH = path;

        // Flush the TX and RX FIFOs
        this.cc1101_strobe_cmd(CC1101_SIDLE);
        this.cc1101_strobe_cmd(CC1101_SFTX);
        this.cc1101_strobe_cmd(CC1101_SFRX);

        if(!this.cc1101_reset())
        {
            console.error(TAG + ":" + "Couldn't reset CC1101");
            return false;
        }

        return this.cc1101_reg_config_settings(pa_level);
    },

    cc1101_set_cli_path(path)
    {
        MCP2210CLI_PATH = path;
    },

    /**
     * Set RX mode
     * @param {*} clear true to clear the RX FIFO
     */
    cc1101_set_rx: function(clear)
    {
        if(clear)
        {
            this.cc1101_strobe_cmd(CC1101_SIDLE);
            this.cc1101_strobe_cmd(CC1101_SFRX);
        }
        this.cc1101_strobe_cmd(CC1101_SRX);
    },

    /**
     * Set TX mode
     */
    cc1101_set_tx: function()
    {
        this.cc1101_strobe_cmd(CC1101_STX);
    },

    /**
     * Set carrier frequency
     * @param {*} mhz frequency in MHz
     * @returns true if successful
     */
    cc1101_set_mhz: function(mhz)
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

        this.spi_write_reg(CC1101_FREQ2, F2);
        this.spi_write_reg(CC1101_FREQ1, F1);
        this.spi_write_reg(CC1101_FREQ0, F0);

        if(this.spi_read_reg(CC1101_FREQ0) != F0 || this.spi_read_reg(CC1101_FREQ1) != F1 || this.spi_read_reg(CC1101_FREQ2) != F2)
        {
            return false;
        }
        return true;
    },

    /**
     * Send data
     * @param {Array} tx_buffer array of bytes to send
     * @returns {Promise} that is resolved when the packet is sent successfully
     */
    cc1101_send_data: function(tx_buffer)
    {
        console.log("Packet size: " + tx_buffer.length);
        if(tx_buffer.length > 61)
        {
            console.warn(TAG + ":" + "Packet too large: %d", tx_buffer.length);
            return new Promise(function(resolve, reject)
            {
                reject();
            });
        }

        // Flush the TX FIFO (go into IDLE first)
        this.cc1101_strobe_cmd(CC1101_SIDLE);
        this.cc1101_strobe_cmd(CC1101_SFTX);
        console.log(TAG + ": " + "TX FIFO before: %d", this.cc1101_bytes_in_tx_fifo());

        this.spi_write_reg(CC1101_TXFIFO, tx_buffer.length);         // Write packet length
        this.spi_write_burst_reg(CC1101_TXFIFO, tx_buffer);          // Write data
        console.log(TAG + ": " + "TX FIFO after: %d", this.cc1101_bytes_in_tx_fifo());

        this.cc1101_set_tx();                                        // Enter TX mode to send the data

        // Internal CC1101 state machine status to check that TX mode has started
        console.log(TAG + ": " + "CC1101 FSM: %d", this.cc1101_read_status(CC1101_MARCSTATE));
        console.log(TAG + ": " + "TX FIFO after: %d", this.cc1101_bytes_in_tx_fifo());
        
        let self = this;
        return new Promise(function(resolve, reject)
        {
            // Wait for GDO0 to be set, indicates sync was transmitted
            self.wait_for_condition(self.cc1101_is_packet_sent_available.bind(self), true, TIMEOUT_SPI).then(function()
            {
                // Wait for GDO0 to be cleared, indicates end of packet
                self.wait_for_condition(self.cc1101_is_packet_sent_available.bind(self), false, TIMEOUT_SPI).then(function()
                {
                    // Packet sent correctly
                    resolve();
                })
                .catch(function()
                {
                    // Failed to wait for end of packet
                    console.error(TAG + ":" + "Failed waiting end of packet");
                    reject();
                });
            })
            .catch(function()
            {
                console.error(TAG + ":" + "Failed waiting sync");
                reject();
            });
        });
    },

    /**
     * Set receiving bandwidth
     * @param {*} bw bandwidth level from 1 to 16
     */
    cc1101_set_rx_bw: function(bw)
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
        this.spi_write_reg(CC1101_MDMCFG4, rx_bw);
    },

    /**
     * Set operating channel
     * @param {*} chn channel
     */
    cc1101_set_channel: function(chn)
    {
        channel = chn;
        this.spi_write_reg(CC1101_CHANNR, channel);
    },

    /**
     * Checks the bytes available in the RX FIFO
     * @returns bytes available in the RX FIFO
     */
    cc1101_bytes_in_rx_fifo: function()
    {
        // Read until we get the same number of bytes in the RX FIFO. This
        //  is needed as a workaround for an errate of the CC1101.
        // Read "SPI Read Synchronization Issue SPI read synchronization 
        //  results in incorrect read values for register fields that are continuously updated"
        //  at http://www.ti.com/lit/er/swrz020e/swrz020e.pdf
        let v1 = 0, v2 = 0;
        for(let n = 0; n < 255; ++n)
        {
            v1 = this.cc1101_read_status(CC1101_RXBYTES);
            v2 = this.cc1101_read_status(CC1101_RXBYTES);
            if(v1 == v2)
            {
                return v1;
            }
        }
        
        // FAILED!
        return -1;
    },

    /**
     * Checks the bytes available in the TX FIFO
     * @returns bytes available in the TX FIFO
     */
    cc1101_bytes_in_tx_fifo: function()
    {
        // Read until we get the same number of bytes in the TX FIFO. This
        //  is needed as a workaround for an errate of the CC1101.
        // Read "SPI Read Synchronization Issue SPI read synchronization 
        //  results in incorrect read values for register fields that are continuously updated"
        //  at http://www.ti.com/lit/er/swrz020e/swrz020e.pdf
        let v1 = 0, v2 = 0;
        for(let n = 0; n < 255; ++n)
        {
            v1 = this.cc1101_read_status(CC1101_TXBYTES);
            v2 = this.cc1101_read_status(CC1101_TXBYTES);
            if(v1 == v2)
            {
                return v1;
            }
        }
        
        // FAILED!
        return -1;
    },

    /**
     * Read data from the RX FIFO
     * @returns packet read
     */
    cc1101_read_data: function()
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
        let rx_bytes = this.cc1101_bytes_in_rx_fifo();

        // Any byte waiting to be read and no overflow?
        if (rx_bytes & 0x7F && !(rx_bytes & 0x80))
        {
            // Read data length. The first byte in the FIFO is the length.
            packet.length = this.spi_read_reg(CC1101_RXFIFO);

            // If packet is too long
            if (packet.length > CC1101_MAX_PACKET_SIZE || packet.length == 0)
            {
                console.warn(TAG + ":" + "Length error: %d", packet.length);
                packet.length = 0;
                return packet;
            }
            else
            {
                let status = [];

                // Read data packet
                packet.data = this.spi_read_burst_reg(CC1101_RXFIFO, packet.length);
                // Read RSSI, LQI and CRC_OK
                status = this.spi_read_burst_reg(CC1101_RXFIFO, status, 2);

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
    },

    /**
     * Read status register
     * @param {*} addr address of register
     * @returns status register
     */
    cc1101_read_status: function(addr)
    {
        // Write 0x00 as a dummy byte, we are interested in the read value
        return this.spi_write_reg(addr | READ_BURST, 0x00);
    },

    /**
     * Check if a packet was sent (when sending packet) or received
     *  (when expecting a packet).
     * @returns true packet was sent or a packet was received
     */
    cc1101_is_packet_sent_available: function()
    {
        let status = this.cc1101_read_status(CC1101_PKTSTATUS);

        // If GDO0 is set a packet was sent or received
        if(status & 0x01 == 1)
        {
            return true;
        }

        return false;
    }
}