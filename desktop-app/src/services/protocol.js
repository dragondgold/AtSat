const cc1101 = require('./cc1101.js');

//***************************************PROTOCOL DEFINES**************************************************//
const START_BYTE = 0x7E;      
const ESCAPE_BYTE = 0x7D;   
const ESCAPE_XOR = 0x20;

const CMD_OK = 0;
const CMD_ERROR_WRONG_FORMAT = 1;
const CMD_ERROR_LENGTH = 2;
const CMD_ERROR_CHECKSUM = 3;

const PROTOCOL_CMD_ERROR                = 0x00;      
const PROTOCOL_CMD_GET_PARACHUTE        = 0x01;      
const PROTOCOL_CMD_SET_PARACHUTE        = 0x02;      
const PROTOCOL_CMD_GET_BALLOON          = 0x03;       
const PROTOCOL_CMD_SET_BALLOON          = 0x04;      
const PROTOCOL_CMD_GET_SENSOR           = 0x05;      
const PROTOCOL_CMD_GET_BATTERY          = 0x06;      
const PROTOCOL_CMD_SET_REPORT_FREQ      = 0x07;     
const PROTOCOL_CMD_START_REPORT    = 0x08;        

const PROTOCOL_SENSOR_ID_VBAT           = 0x01;
const PROTOCOL_SENSOR_ID_IBAT           = 0x02;
const PROTOCOL_SENSOR_ID_V3V3           = 0x03;
const PROTOCOL_SENSOR_ID_I3V3           = 0x04;
const PROTOCOL_SENSOR_ID_V5             = 0x05;
const PROTOCOL_SENSOR_ID_I5             = 0x06;
const PROTOCOL_SENSOR_ID_GPS            = 0x07; 
const PROTOCOL_SENSOR_ID_GYROSCOPE      = 0x08; 
const PROTOCOL_SENSOR_ID_MAGNETOMETER   = 0x09;
const PROTOCOL_SENSOR_ID_ACCELEROMETER  = 0x0A;
const PROTOCOL_SENSOR_ID_ORIENTATION    = 0x0B;
const PROTOCOL_SENSOR_ID_TEMPERATURE    = 0x0C;
const PROTOCOL_SENSOR_ID_HUMIDITY       = 0x0D;
const PROTOCOL_SENSOR_ID_PRESSURE       = 0x0E;
const PROTOCOL_SENSOR_ID_ALTITUDE       = 0x0F;

const sendCMDLength =
[
    { name: 'getError', id: PROTOCOL_CMD_ERROR, length: 1 },                // send: CMD
    { name: 'getParachute', id: PROTOCOL_CMD_GET_PARACHUTE, length: 1 },    // send: CMD
    { name: 'setParachute', id: PROTOCOL_CMD_SET_PARACHUTE, length: 2 },    // send: CMD + SETTER
    { name: 'getBalloon', id: PROTOCOL_CMD_GET_BALLOON, length: 1 },      // send: CMD
    { name: 'setBalloon', id: PROTOCOL_CMD_SET_BALLOON, length: 2 },          // send: CMD + SETTER
    { name: 'getSensor', id: PROTOCOL_CMD_GET_SENSOR, length: 'variable' },
    { name: 'getBattery', id: PROTOCOL_CMD_GET_BATTERY, length: 1 },      // send: CMD
    { name: 'setReport', id: PROTOCOL_CMD_SET_REPORT_FREQ, length: 2 },  // send: CMD + SETTER
    { name: 'startReport', id: PROTOCOL_CMD_START_REPORT, length: 2 }      // send: CMD + SETTER
]

let recCMDLength = 
[
    { id: PROTOCOL_CMD_ERROR,               length: 2 }, // rec: CMD + ERROR CODE            
    { id: PROTOCOL_CMD_GET_PARACHUTE,       length: 2 }, // rec: CMD + PARACHUTE STATE       
    { id: PROTOCOL_CMD_SET_PARACHUTE,       length: 2 }, // rec: CMD + STATE                
    { id: PROTOCOL_CMD_GET_BALLOON,         length: 2 }, // rec: CMD + PARACHUTE STATE       
    { id: PROTOCOL_CMD_SET_BALLOON,         length: 2 }, // rec: CMD + STATE                 
    { id: PROTOCOL_CMD_GET_SENSOR,          length: 'variable' }, // Explained below
    { id: PROTOCOL_CMD_GET_BATTERY,         length: 2 }, // rec: CMD + BATTERY %             
    { id: PROTOCOL_CMD_SET_REPORT_FREQ,     length: 2 }, // rec: CMD + STATE                 
    { id: PROTOCOL_CMD_START_REPORT,        length: 2 } // rec: CMD + STATE        
]         

let sensorsLength = 
[
    { id: PROTOCOL_SENSOR_ID_VBAT,           length: 2 }, // rec: MSB mV + LSB mV     send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_IBAT,           length: 2 }, // rec: MSB mA + LSB mA     send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_V3V3,           length: 2 }, // rec: MSB mV + LSB mV     send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_I3V3,           length: 2 }, // rec: MSB mA + LSB mA     send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_V5,             length: 2 }, // rec: MSB mV + LSB mV     send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_I5,             length: 2 }, // rec: MSB mA + LSB mA     send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_GPS,            length: 8 }, // rec: LAT MSB + LAT[2] + LAT[1] + LAT LSB + LNG MSB + LNG[2] + LNG[1] + LNG LSB    send: CMD
    { id: PROTOCOL_SENSOR_ID_GYROSCOPE,      length: 6 }, // rec: MSB x + LSB x + MSB y + LSB y + MSB z + LSB z   send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_MAGNETOMETER,   length: 6 }, // rec: MSB x + LSB x + MSB y + LSB y + MSB z + LSB z   send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_ACCELEROMETER,  length: 6 }, // rec: MSB x + LSB x + MSB y + LSB y + MSB z + LSB z   send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_ORIENTATION,    length: 6 }, // rec: MSB x + LSB x + MSB y + LSB y + MSB z + LSB z   send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_TEMPERATURE,    length: 1 }, // rec: TEMP °              send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_HUMIDITY,       length: 1 }, // rec: HUM %               send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_PRESSURE,       length: 2 }, // rec: MSB p + LSB p       send: CMD + ID 
    { id: PROTOCOL_SENSOR_ID_ALTITUDE,       length: 2 }, // rec: MSB a + LSB a       send: CMD + ID 
]




let recPackets = 
{
    cmd: 0, 
    len: 0,
    error: 0,  
    funcName: '',
    packetsToDecode: [],
    decoded: []
};

/**
 * Clear cmd_decoded
 */
protocol_clear_decoded = function()
{
    recPackets.cmd = 0;
    recPackets.len = 0;
    recPackets.error = 0;  
    recPackets.funcName = '';  
    recPackets.packetsToDecode = []; 
    recPackets.decoded = [];
}

/**
 * Calculate checksum of data
 * @param {*} data array of data to calculate checksum
 * @returns value of checksum 
 */
protocol_calculate_checksum = function(data)
{
    let checksum = 0;
    for(let n = 0; n < data.length; n++){
        checksum += data[n];
    }
    return 0xff - (checksum & 0xff);
}

/**
 * Add escape to packet
 * @param {*} packet to modify
 * @returns valid packet
 */
protocol_add_escape = function(packet)
{
    let newPacket = [];
    newPacket.push(packet[0]);
    for(let b = 1 ; b < packet.length; b++){
        if(packet[b] == START_BYTE || packet[b] == ESCAPE_BYTE){
            newPacket.push(ESCAPE_BYTE);
            newPacket.push(packet[b]  ^ ESCAPE_XOR);
        }else{
            newPacket.push(packet[b]);
        }
    }
    return newPacket
}

/**
 * Remove escape from pkg
 * @returns clean packet
 */
protocol_remove_escape = function(packet)
{
    let escape = false;
    let newPacket = [];
    for(let b = 0; b < packet.length; b++){
        if(packet[b] == ESCAPE_BYTE){
            escape =true;
        }else{    
            if(escape == true){
                escape = false;
                newPacket.push(packet[b] ^ ESCAPE_XOR);
            }else{
                newPacket.push(packet[b]);
            }
        }
    }
    return newPacket
}

/**
 * Decode data
 * @param {*} packets arrays to decode
 * @returns object recPackets
 */
protocol_decode_data = function(packets)
{
    let cmd = recPackets.cmd;
    let length = recPackets.len;

    if(protocol_split_valid_packets(packets)){
        let packetsToDecode = recPackets.packetsToDecode;
        for(let p = 0; p < packetsToDecode.length; p++)
        {
            let packet = packetsToDecode[p];
            let cmd = packet[0];

            switch(cmd)
            {
                case PROTOCOL_CMD_ERROR:
                case PROTOCOL_CMD_GET_PARACHUTE:
                case PROTOCOL_CMD_SET_PARACHUTE:
                case PROTOCOL_CMD_GET_BALLOON:
                case PROTOCOL_CMD_SET_BALLOON:
                case PROTOCOL_CMD_GET_BATTERY:
                case PROTOCOL_CMD_SET_REPORT_FREQ:
                    recPackets.decoded.push(
                        { state : packet[1], cmd: cmd}
                    );
                    recPackets.error = CMD_OK;   
                    break;
                case PROTOCOL_CMD_GET_SENSOR:
                    let id = packet[1];
                    let value = 0;
                    
                    switch(id)
                    {
                        case PROTOCOL_SENSOR_ID_GYROSCOPE:
                        case PROTOCOL_SENSOR_ID_MAGNETOMETER:
                        case PROTOCOL_SENSOR_ID_ACCELEROMETER:
                        case PROTOCOL_SENSOR_ID_ORIENTATION:
                            
                            let x = protocol_convert_to_signed(packet[2] * Math.pow(2,8) + packet[3]);
                            let y = protocol_convert_to_signed(packet[4] * Math.pow(2,8) + packet[5]);
                            let z = protocol_convert_to_signed(packet[6] * Math.pow(2,8) + packet[7]);

                            recPackets.decoded.push(
                                { 
                                    cmd: cmd ,
                                    sensorID : id ,
                                    x : x ,
                                    y : y ,
                                    z : z
                                }
                            );
                            break;
                        case PROTOCOL_SENSOR_ID_TEMPERATURE:
                        case PROTOCOL_SENSOR_ID_HUMIDITY:
                            value = packet[2];
                            recPackets.decoded.push(
                                { 
                                    cmd : cmd ,
                                    sensorID : id ,
                                    value : value 
                                }
                            );
                            break;
                        case PROTOCOL_SENSOR_ID_PRESSURE:
                        case PROTOCOL_SENSOR_ID_ALTITUDE:
                        case PROTOCOL_SENSOR_ID_VBAT:
                        case PROTOCOL_SENSOR_ID_IBAT:
                        case PROTOCOL_SENSOR_ID_V3V3:
                        case PROTOCOL_SENSOR_ID_I3V3:
                        case PROTOCOL_SENSOR_ID_V5:
                        case PROTOCOL_SENSOR_ID_I5:

                            value = packet[2] * Math.pow(2,8) + packet[3];
                            recPackets.decoded.push(
                                { 
                                    cmd : cmd,
                                    sensorID : id ,
                                    value : value 
                                }
                            );
                            break;
                        case PROTOCOL_SENSOR_ID_GPS:
                            let latInt = packet[2] * Math.pow(2,24) + packet[3] * Math.pow(2,16) + packet[4] * Math.pow(2,8) + packet[5];
                            let lngInt = packet[6] * Math.pow(2,24) + packet[7] * Math.pow(2,16) + packet[8] * Math.pow(2,8) + packet[9];
                
                            let lat = ((~~latInt) / (2147483647 / 180.0));
                            let lng = ((~~lngInt) / (0x7FFFFFFF / 180.0));
                                
                            recPackets.decoded.push(
                                { 
                                    cmd : cmd ,
                                    sensorID : id ,
                                    lat : lat ,
                                    lng : lng  
                                }
                            );

                            break; 
                    }
                    recPackets.error = CMD_OK; 
                    break; 
                        
            }
        }
    }
    return recPackets
}

/**
 * Split received buffer in an array of packet to decode
 * @param {*} rx_buffer to split
 * @returns true if we have packet to decode. recPackets object contains error info.
 */
protocol_split_valid_packets = function(rx_buffer)
{
    if( recPackets.cmd == PROTOCOL_CMD_GET_SENSOR )
    {
        let data = rx_buffer.slice(4,rx_buffer.length-1); // Remove start byte, length, cmd and checksum
        let index = 0;
        let splitIndex= [];
        let packetsToDecode = [];
        while(index < data.length)
        {
            let sensorIndex = index;
            let sensorID = data[sensorIndex];

            let validID = sensorsLength.filter( function(n) 
            {
                return (n.id == sensorID)
            });
            
            if( validID[0] )
            {
                index +=  validID[0].length + 1;
                splitIndex.push(index);
                // complete
                if( index == data.length ) 
                {
                    break;
                }
                // Missing bytes
                if( index > data.length )
                {
                    recPackets.funcName = protocol_split_valid_packets.name
                    recPackets.error = CMD_ERROR_LENGTH;
                    return false;
                }
            }
            else
            {
                recPackets.funcName = protocol_split_valid_packets.name
                recPackets.error = CMD_ERROR_WRONG_FORMAT;
                return false;
            }     
        }

        for( let p = 0 ; p < splitIndex.length; p ++ )
        {
            let lowerindex = 0;
            if(p != 0){
                lowerindex = splitIndex[p-1]
            }
            let index = splitIndex[p];
            packetsToDecode.push( data.slice(lowerindex, index));
            packetsToDecode[packetsToDecode.length -1].splice(0, 0, recPackets.cmd);
        }
        recPackets.packetsToDecode = packetsToDecode;
    }
    else
    {
        // Validate ID
        let validID = recCMDLength.filter( function(n) 
        {
            return (n.id == recPackets.cmd)
        });
        
        if(validID[0]){
            // Validate length
            if( validID[0].length ==  recPackets.len){
                recPackets.packetsToDecode.push(rx_buffer.slice(3, rx_buffer.length -1));
            }else{
                recPackets.funcName = protocol_split_valid_packets.name
                recPackets.error = CMD_ERROR_LENGTH;
                return false;
            }
        }else{
            recPackets.funcName = protocol_split_valid_packets.name
            recPackets.error = CMD_ERROR_WRONG_FORMAT;
            return false;
        }
    }
    recPackets.error = CMD_OK;
    return true;
}


/**
 * Get the valid packet from received buffer
 * @param {*} rx_buffer to format
 * @returns formatted packet
 */
protocol_extract_packet = function(rx_buffer)
{
    let escape = false;
    let packet = [];
    
    let startIndex = rx_buffer.indexOf(START_BYTE);

    // Find START_BYTE 
    if(startIndex >= 0 && startIndex + 4 <= rx_buffer.length){
        packet = rx_buffer.slice(startIndex);
        let length = packet[1] * 8 + packet[2];
        // Check length
        if(length + 4 == packet.length){
            recPackets.len = length;
            let cmd = packet[3];

            // check cmd
            let validID = recCMDLength.filter(function(n) {
                return (n.id == cmd)
            });
            
            // Checksum
            let checksum = 0;
            if(validID[0]){
                recPackets.cmd = cmd;
                
                for(let b = 3; b < packet.length; b++){
                    checksum += packet[b];
                }
                checksum = checksum & 0xff;

                if(checksum == 0xff){
                    recPackets.checksum = checksum;
                }else{
                    recPackets.funcName = protocol_extract_packet.name
                    recPackets.error = CMD_ERROR_CHECKSUM;
                    packet = [];
                }
            } else{
                recPackets.funcName = protocol_extract_packet.name
                recPackets.error = CMD_ERROR_WRONG_FORMAT;
                packet = [];
            } 
        }else{
            recPackets.funcName = protocol_extract_packet.name
            recPackets.error = CMD_ERROR_LENGTH;
            packet = [];
        }
    }
    return packet;
}

/**
 * Create packet 
 * @param {*} data array to format where data[0] is cmd 
 * @returns packet with valid protocol
 */
protocol_format_packet =  function(data)
{
    let packet = [];
    let length = data.length; // Because cmd is included in data as first element of array
    let checksum = protocol_calculate_checksum(data);
    packet.push(START_BYTE);
    packet.push((length >> 8) & 0xFF);
    packet.push(length & 0xFF);
    for(let d = 0 ; d < data.length; d++){
        packet.push(data[d]);
    }
    packet.push(checksum);

    packet = protocol_add_escape(packet);
    return packet;
}


/**
 * Format packet
 * @param {*} packet to format
 * @returns formatted packet
 */
protocol_convert_to_signed = function(value)
{
    if ((value & 0x8000) > 0) {
        value = value - 0x10000;
    }
    return value;
}

module.exports =
{

    /**
     * Create packet 
     * @param {*} cmdName of packet to send
     * @param {*} data 
     * @returns packet with valid protocol
     */
    create_packet: function(cmdName, data)
    {
        let packet = [];
    
        let cmdValid = sendCMDLength.filter( function(n) 
        {
            return (n.name == cmdName)
        });
        
        if(cmdValid[0])
        {
            if(cmdValid[0].length == 'variable'){
                packet.push(cmdValid[0].id);
                for(let d = 0 ; d < data.length ; d++ )
                {
                    packet.push(data[d]);
                }
            }else{
                if(cmdValid[0].length == data.length + 1) 
                {
                    packet.push(cmdValid[0].id);
                    for(let d = 0 ; d < data.length ; d++ )
                    {
                        packet.push(data[d]);
                    }
                }
                else
                {
                    recPackets.funcName = this.create_packet.name
                    recPackets.error = CMD_ERROR_LENGTH;
                    return packet;
                }
            }        
        }
        return protocol_format_packet(packet);
    },

    /**
     * Decode data from packet
     * @param {*} packet to decode
     * @returns Object with decoding data
     */
    decode_packet: function(packet)
    {
        if(packet.data.length < 5){
            recPackets.funcName = this.decode_packet.name
            recPackets.error = CMD_ERROR_LENGTH;
            return recPackets;
        }

        let pkg = protocol_remove_escape(packet.data);

        protocol_clear_decoded();

        let newPacket = protocol_extract_packet(pkg);

        if(newPacket.length == 0){
            return recPackets;
        }

        return protocol_decode_data(newPacket);
    },
}