const SerialPort = require("serialport");

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

let channel = 1;
let port = null;

const TAG = "CC1101";
const START_BYTE = 0x7E;      
const ESCAPE_BYTE = 0x7D;   
const ESCAPE_XOR = 0x20;

var protocol_add_escape = function(packet)
{
    let newPacket = [];
    newPacket.push(packet[0]);
	
    for(let b = 1 ; b < packet.length; b++)
	{
        if(packet[b] == START_BYTE || packet[b] == ESCAPE_BYTE)
		{
            newPacket.push(ESCAPE_BYTE);
            newPacket.push(packet[b]  ^ ESCAPE_XOR);
        }
		else
		{
            newPacket.push(packet[b]);
        }
    }
	
    return newPacket
}

var protocol_remove_escape = function(packet)
{
    let escape = false;
    let newPacket = [];
	
    for(let b = 0; b < packet.length; b++)
	{
        if(packet[b] == ESCAPE_BYTE)
		{
            escape = true;
        }
		else
		{    
            if(escape == true)
			{
                escape = false;
                newPacket.push(packet[b] ^ ESCAPE_XOR);
            }
			else
			{
                newPacket.push(packet[b]);
            }
        }
    }
	
    return newPacket
}

/**
 * Calculate checksum of data
 * @param {*} data array of data to calculate checksum
 * @returns value of checksum 
 */
protocol_calculate_checksum = function(data)
{
    let checksum = 0;
    for(let n = 0; n < data.length; n++)
    {
        checksum += data[n];
    }
    return 0xff - (checksum & 0xff);
}

/**
 * Create packet 
 * @param {*} data array to format where data[0] is cmd 
 * @returns packet with valid protocol
 */
protocol_format_packet = function(data)
{
    // Because cmd is included in data as first element of array
    let packet = [];
    let length = data.length;
    let checksum = protocol_calculate_checksum(data);

    packet.push(START_BYTE);
    packet.push(length);

    for(let d = 0 ; d < data.length; d++)
    {
        packet.push(data[d]);
    }
    packet.push(checksum);

    packet = protocol_add_escape(packet);
    return packet;
}

protocol_get_data = function(packet)
{
    let bytes = protocol_remove_escape(packet);
    //console.log("Removed escape: " + bytes);

    for(let n = 0; n < bytes.length; ++n)
    {
        if(bytes[n] == START_BYTE && n + 1 < bytes.length)
        {
            let length = bytes[n + 1];
            //console.log("Length: " + length);
            if(n + 1 + length + 1 < bytes.length)
            {
                //console.log("Getting data");
                // Get data from the packet
                let data = bytes.slice(n + 2, n + 2 + length);
                //console.log("Got data: " + data);

                // Get checksum
                let checksum = bytes[n + 2 + length];
                //console.log("Got checksum: " + checksum);

                // Add all the bytes in the data
                let sum = data.reduce((partial_sum, a) => partial_sum + a); 
                if((sum + checksum) & 0xFF == 0xFF)
                {
                    return data;
                }
            }
        }
    }

    return [];
}

function toHexString(byteArray) {
    return Array.from(byteArray, function(byte) {
      return ('0' + (byte & 0xFF).toString(16)).slice(-2);
    }).join(' ')
  }

module.exports =
{
	
    /**
     * Set CS pin level
     * @param {*} level true or false
     */
    set_cs_level: async function(level)
    {
        return true;
    },

    /**
     * Transfer data to the SPI
     * @param {Array} data data to send
     * @returns {Object} object with the parameters .data that contains the read data and
     *  .error that is true if an error ocurred.
     */
    mcp2210_transfer_data: async function(data)
    {
        return new Promise(function(resolve, reject)
        {
            //console.log("mcp2210_transfer_data()");
            function data_rcv(data)
            {
                let obj = 
                {
                    error: false,
                    data: []
                }

                //console.log("Received: " + toHexString(data));
                obj.data = protocol_get_data(data).slice(1);
                //console.log("Decoded: " + obj.data);
                port.removeAllListeners('data');
                //port.removeListener('data', data_rcv);
                resolve(obj);
            }

            // Set listener
            port.on('data', function(data)
            {
                data_rcv(data);
            });

            // Send the data and wait for the reply
            let tx_data = Buffer.from(protocol_format_packet([0x01].concat(data)));
            //console.log("Data: " + toHexString(data));
            //console.log("Will send: " + toHexString(tx_data));
            port.write(tx_data);
        });
    },

    /**
     * Write a register
     * @param {*} addr Address of the register
     * @param {*} value Value to write in the register
     * @returns read-back value
     */
    spi_write_reg: async function(addr, value)
    {
        let obj = await this.mcp2210_transfer_data([addr, value]);

        if(!obj.error)
        {
            return obj.data[1];
        }
        
        return 0;
    },

    /**
     * Send a command
     * @param {*} cmd command to send
     */
    spi_send_cmd: async function(cmd)
    {
        return await this.mcp2210_transfer_data([cmd]);
    },

    /**
     * Write a register in burst mode
     * @param {*} addr Address of the register
     * @param {*} buffer Array of bytes to send
     */
    spi_write_burst_reg: async function(addr, buffer)
    {
        var obj = await this.mcp2210_transfer_data([addr | WRITE_BURST].concat(buffer));

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
    spi_read_reg: async function(addr)
    {
        let obj = await this.mcp2210_transfer_data([addr | READ_SINGLE, 0x00]);
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
    spi_read_burst_reg: async function(addr, length)
    {
        let data = [addr | READ_BURST];
        for(let n = 0; n < length; ++n)
        {
            // Add dummy bytes to read
            data.push(0x00);
        }

        let obj = this.mcp2210_transfer_data(data);

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
    cc1101_reset: async function()
    {
        async function delay(duration) {
            return function()
            {
                return new Promise(function(resolve, reject)
                {
                    setTimeout(function()
                    {
                        resolve();
                    }, duration)
                });
            };
        }

        await this.set_cs_level(false);
        await delay(10);
        await this.set_cs_level(true);
        await delay(10);
        await this.set_cs_level(false);
        await delay(10);

        // Reset the transceiver
        await this.spi_send_cmd(CC1101_SRES);

        // Wait for reset
        await delay(10);

        await this.set_cs_level(true);
        return true;
    },

    /**
     * Send a strobe command
     * @param {*} strobe strobe command to send
     * @returns Promise that resolves if command executed
     */
    cc1101_strobe_cmd: async function(strobe)
    {
        let s1 = await this.set_cs_level(false);
        let s2 = await this.spi_send_cmd(strobe);
        let s3 = await this.set_cs_level(true);

        return s1 && s2 && s3;
    },

    /**
     * Set the default configuration for this driver
     * @returns true if configuration was successful
     */
    cc1101_reg_config_settings: async function()
    {
        await this.spi_write_reg(CC1101_FSCTRL1, 0x06);
        await this.spi_write_reg(CC1101_FSCTRL0, 0x00);
        
        await this.spi_write_burst_reg(CC1101_PATABLE, [0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00]);

        await this.spi_write_reg(CC1101_FIFOTHR,  0x47);
        await this.spi_write_reg(CC1101_MDMCFG4,  0xC8);   // Baud rate 10k
        await this.spi_write_reg(CC1101_MDMCFG3,  0x93);   // Baud rate 1C3
        await this.spi_write_reg(CC1101_MDMCFG2,  0x13);   // 30/32 sync word, no Manchester encoding, GFSK modulation, DC filter before modulator
        await this.spi_write_reg(CC1101_MDMCFG1,  0x72);   // 4 preamble bytes, no forward error correction
        await this.spi_write_reg(CC1101_MDMCFG0,  0xF8);   // 200 kHz channel spacing together with CHANSPC_E bits in MDMCFG1
        await this.spi_write_reg(CC1101_CHANNR,   channel);// Channel number
        await this.spi_write_reg(CC1101_DEVIATN,  0x40);
        await this.spi_write_reg(CC1101_FREND1,   0x56);
        await this.spi_write_reg(CC1101_FREND0,   0x10);
        await this.spi_write_reg(CC1101_MCSM0,    0x18);
        await this.spi_write_reg(CC1101_FOCCFG,   0x16);
        await this.spi_write_reg(CC1101_BSCFG,    0x6C);
        await this.spi_write_reg(CC1101_AGCCTRL2, 0x43);
        await this.spi_write_reg(CC1101_AGCCTRL1, 0x40);
        await this.spi_write_reg(CC1101_AGCCTRL0, 0x91);
        await this.spi_write_reg(CC1101_FSCAL3,   0xE9);      // Value given by TI SmartRF Studio
        await this.spi_write_reg(CC1101_FSCAL2,   0x2A);      // Value given by TI SmartRF Studio
        await this.spi_write_reg(CC1101_FSCAL1,   0x00);      // Value given by TI SmartRF Studio
        await this.spi_write_reg(CC1101_FSCAL0,   0x1F);      // Value given by TI SmartRF Studio
        await this.spi_write_reg(CC1101_FSTEST,   0x59);   
        await this.spi_write_reg(CC1101_TEST2,    0x81);      // Value given by TI SmartRF Studio
        await this.spi_write_reg(CC1101_TEST1,    0x35);      // Value given by TI SmartRF Studio
        await this.spi_write_reg(CC1101_TEST0,    0x09);      // Value given by TI SmartRF Studio
        await this.spi_write_reg(CC1101_IOCFG2,   0x2E);      // Serial clock is synchronous to the data in synchronous serial mode
        await this.spi_write_reg(CC1101_IOCFG0,   0x06);      // Asserts GDO0 when sync word has been sent/received, and de-asserts at the end of the packet 
        await this.spi_write_reg(CC1101_PKTCTRL1, 0x04);      // Two status bytes will be appended to the payload of the packet, including RSSI, LQI and CRC OK
                                                                // No address check
        await this.spi_write_reg(CC1101_PKTCTRL0, 0x41);	    // Whitening OFF, CRC Enabled, variable length packets, packet length configured by the first byte after sync word
        await this.spi_write_reg(CC1101_ADDR,     0x00);	    // Address used for packet filtration (not used here)
        await this.spi_write_reg(CC1101_PKTLEN,   0x3D); 	    // 61 bytes max packet length allowed
        await this.spi_write_reg(CC1101_MCSM1,    0x3F);

        // Set frequency to 915 MHz (values taken from SmartRF Studio)
        await this.spi_write_reg(CC1101_FREQ2, 0x23);
        await this.spi_write_reg(CC1101_FREQ1, 0x31);
        await this.spi_write_reg(CC1101_FREQ0, 0x3B);

        // Check the registers values
        let registers = [   CC1101_FSCTRL1, 0x06,
                            CC1101_FSCTRL0, 0x00,
                            CC1101_MDMCFG4, 0xC8,
                            CC1101_MDMCFG3, 0x93,
                            CC1101_MDMCFG2, 0x13,
                            CC1101_MDMCFG1, 0x72,
                            CC1101_MDMCFG0, 0xF8,
                            CC1101_CHANNR, channel,
                            CC1101_DEVIATN, 0x40,
                            CC1101_FREND1, 0x56,
                            CC1101_FREND0, 0x10,
                            CC1101_MCSM0, 0x18,
                            CC1101_FOCCFG, 0x16,
                            CC1101_BSCFG, 0x6C,
                            CC1101_AGCCTRL2, 0x43,
                            CC1101_AGCCTRL1, 0x40,
                            CC1101_AGCCTRL0, 0x91,
                            CC1101_FSCAL3, 0xE9,
                            CC1101_FSCAL2, 0x2A,
                            CC1101_FSCAL1, 0x00,
                            CC1101_FSCAL0, 0x1F,
                            CC1101_FSTEST, 0x59,
                            CC1101_TEST2, 0x81,
                            CC1101_TEST1, 0x35,
                            CC1101_TEST0, 0x09,
                            CC1101_IOCFG2, 0x2E,
                            CC1101_IOCFG0, 0x06,
                            CC1101_PKTCTRL1, 0x04,
                            CC1101_PKTCTRL0, 0x41,
                            CC1101_ADDR, 0x00,
                            CC1101_PKTLEN, 0x3D,
                            CC1101_FREQ2, 0x23,
                            CC1101_FREQ1, 0x31,
                            CC1101_FREQ0, 0x3B,
                            CC1101_MCSM1, 0x3F
                        ];

        for(let n = 0; n < registers.length - 1; n += 2)
        {
            let val = 0;
            if((val = await this.spi_read_reg(registers[n])) != registers[n + 1])
            {
                console.error("Error on register " + registers[n] + ".Read: " + val + " instead of " + registers[n + 1]);
                return false;
            }
        }

        return true;
    },

    /**
     * Init the CC1101 transceiver
     * @param port serial port to use
     * @returns Promise resolving if init was successful
     */
    cc1101_init: async function(serial_port_name)
    {
        var self = this;

        return new Promise(function(resolve, reject)
        {
            // Function to init the CC1101
            async function setup()
            {
                console.log('Port ' + serial_port_name + ' opened!');
                
                console.time("init");
                // Flush the TX and RX FIFOs
                await self.cc1101_strobe_cmd(CC1101_SIDLE);
                await self.cc1101_strobe_cmd(CC1101_SFTX);
                await self.cc1101_strobe_cmd(CC1101_SFRX);
        
                // Reset the CC1101
                //await self.cc1101_reset();
        
                let r = await self.cc1101_reg_config_settings();
                console.timeEnd("init");
                resolve(r);
            }

            port = new SerialPort(serial_port_name, {
                baudRate: 115200
            });
            
            port.on("open", function () {
                // Init the CC1101 now that the port is open
                setup();
            });
    
            // open errors will be emitted as an error event 
            port.on('error', function(err) {
                reject(err);
            })
        });
    },

    /**
     * Set RX mode
     * @param {*} clear true to clear the RX FIFO
     */
    cc1101_set_rx: async function(clear)
    {
        if(clear)
        {
            await this.cc1101_strobe_cmd(CC1101_SIDLE);
            await this.cc1101_strobe_cmd(CC1101_SFRX);
        }
        await this.cc1101_strobe_cmd(CC1101_SRX);
    },

    /**
     * Set TX mode
     */
    cc1101_set_tx: async function()
    {
        await this.cc1101_strobe_cmd(CC1101_STX);
    },

    /**
     * Send data
     * @param {Array} tx_buffer array of bytes to send
     * @returns {Promise} that is resolved when the packet is sent successfully
     */
    cc1101_send_data: async function(tx_buffer)
    {
        //console.log("Packet size: " + tx_buffer.length);
        if(tx_buffer.length > 61)
        {
            console.warn(TAG + ":" + "Packet too large: %d", tx_buffer.length);
            return false;
        }

        // Flush the TX FIFO (go into IDLE first)
        await this.cc1101_strobe_cmd(CC1101_SIDLE);
        await this.cc1101_strobe_cmd(CC1101_SFTX);
        //console.log(TAG + ": " + "TX FIFO before: %d", this.cc1101_bytes_in_tx_fifo());

        await this.spi_write_reg(CC1101_TXFIFO, tx_buffer.length);      // Write packet length
        await this.spi_write_burst_reg(CC1101_TXFIFO, tx_buffer);       // Write data
        //console.log(TAG + ": " + "TX FIFO after: %d", this.cc1101_bytes_in_tx_fifo());

        await this.cc1101_set_tx();     // Enter TX mode to send the data

        // Internal CC1101 state machine status to check that TX mode has started
        //console.log(TAG + ": " + "CC1101 FSM: %d", this.cc1101_read_status(CC1101_MARCSTATE));
        //console.log(TAG + ": " + "TX FIFO after: %d", this.cc1101_bytes_in_tx_fifo());
        
        //TODO: check for packet 
        // This doesn't work, we should find a way to detect when the packet has been sent
        //while(this.cc1101_is_packet_sent_available());
        //while(!this.cc1101_is_packet_sent_available());

        return true;
    },

    /**
     * Set operating channel
     * @param {*} chn channel
     */
    cc1101_set_channel: async function(chn)
    {
        channel = chn;
        return await this.spi_write_reg(CC1101_CHANNR, channel);
    },

    /**
     * Checks the bytes available in the RX FIFO
     * @returns bytes available in the RX FIFO
     */
    cc1101_bytes_in_rx_fifo: async function()
    {
        // Read until we get the same number of bytes in the RX FIFO. This
        //  is needed as a workaround for an errate of the CC1101.
        // Read "SPI Read Synchronization Issue SPI read synchronization 
        //  results in incorrect read values for register fields that are continuously updated"
        //  at http://www.ti.com/lit/er/swrz020e/swrz020e.pdf
        let v1 = 0, v2 = 0;
        for(let n = 0; n < 255; ++n)
        {
            v1 = await this.cc1101_read_status(CC1101_RXBYTES);
            v2 = await this.cc1101_read_status(CC1101_RXBYTES);
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
    cc1101_bytes_in_tx_fifo: async function()
    {
        // Read until we get the same number of bytes in the TX FIFO. This
        //  is needed as a workaround for an errate of the CC1101.
        // Read "SPI Read Synchronization Issue SPI read synchronization 
        //  results in incorrect read values for register fields that are continuously updated"
        //  at http://www.ti.com/lit/er/swrz020e/swrz020e.pdf
        let v1 = 0, v2 = 0;
        for(let n = 0; n < 255; ++n)
        {
            v1 = await this.cc1101_read_status(CC1101_TXBYTES);
            v2 = await this.cc1101_read_status(CC1101_TXBYTES);
            if(v1 == v2)
            {
                return v1;
            }
        }
        
        // FAILED!
        return -1;
    },

    /**
     * Check if the RX FIFO has overflowed
     * @returns true if overflow occurred
     */
    cc1101_is_rx_overflow: async function()
    {
        if(await this.cc1101_bytes_in_rx_fifo() & 0x80)
        {
            return true;
        }
        return false;
    },

    cc1101_flush_rx_fifo: async function()
    {
        await this.cc1101_strobe_cmd(CC1101_SFRX);
    },

    /**
     * Read data from the RX FIFO
     * @returns packet read
     */
    cc1101_read_data: async function()
    {
        // Create packet here
        let packet = 
        {
            length: 0,
            data: [],
            rssi: 0,
            lqi: 0,
            valid: false,
        }

        // Read the number of bytes in the RX FIFO
        let rx_bytes = await this.cc1101_bytes_in_rx_fifo();

        // Any byte waiting to be read?
        if (rx_bytes & 0x7F)
        {
            // Read data length. The first byte in the FIFO is the length.
            packet.length = await this.spi_read_reg(CC1101_RXFIFO);

            // If packet is too long
            if (packet.length > CC1101_MAX_PACKET_SIZE || packet.length == 0)
            {
                console.warn(TAG + ": " + "Length error: %d", packet.length);
                packet.length = 0;

                // Overflow? Clear the buffer
                if(rx_bytes & 0x80)
                {
                    await this.cc1101_set_rx(true);
                }
                return packet;
            }
            else
            {
                let status = [];

                // Read data packet
                packet.data = await this.spi_read_burst_reg(CC1101_RXFIFO, packet.length);
                // Read RSSI and LQI
                status = await this.spi_read_burst_reg(CC1101_RXFIFO, 2);

                packet.rssi = status[0] >= 128 ? ((status[0] - 256) / 2 - 74) : ((status[0] / 2) - 74);
                packet.lqi = status[1] & 0x7F;
                packet.valid = true;

                // The packet->data contains the data but the data[0] has the status bytes that
                //  is received when the spi_read_burst_reg() addresses the RX FIFO so we have to
                //  shift everything one place to the left
                packet.data = packet.data.splice(1);

                // Overflow? Clear the buffer
                if(rx_bytes & 0x80)
                {
                    await this.cc1101_set_rx(true);
                }

                return packet;
            }
        }
        else
        {
            // Overflow? Clear the buffer
            if(rx_bytes & 0x80)
            {
                await this.cc1101_set_rx(true);
            }

            return packet;
        }
    },

    /**
     * Read status register
     * @param {*} addr address of register
     * @returns status register
     */
    cc1101_read_status: async function(addr)
    {
        // Write 0x00 as a dummy byte, we are interested in the read value
        return await this.spi_write_reg(addr | READ_BURST, 0x00);
    },

    /**
     * Check if a packet was sent (when sending packet) or received
     *  (when expecting a packet).
     * @returns true packet was sent or a packet was received
     */
    cc1101_is_packet_sent_available: async function()
    {
        let status = await this.cc1101_read_status(CC1101_PKTSTATUS);

        // If GDO0 is set a packet was sent or received
        if(status & 0x01 == 1)
        {
            return false;
        }

        return true;
    }
}