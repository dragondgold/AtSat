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
const PROTOCOL_CMD_START_STOP_REPORT    = 0x08;      
const PROTOCOL_CMD_GET_GPS              = 0x09;  

const PROTOCOL_SENSOR_ID_GYROSCOPE      = 0x01; 
const PROTOCOL_SENSOR_ID_MAGNETOMETER   = 0x02;
const PROTOCOL_SENSOR_ID_ACCELEROMETER  = 0x03;
const PROTOCOL_SENSOR_ID_ORIENTATION    = 0x04;
const PROTOCOL_SENSOR_ID_TEMPERATURE    = 0x05;
const PROTOCOL_SENSOR_ID_HUMIDITY       = 0x06;
const PROTOCOL_SENSOR_ID_PRESSURE       = 0x07;
const PROTOCOL_SENSOR_ID_ALTITUDE       = 0x08;
const PROTOCOL_SENSOR_ID_VBAT           = 0x09;
const PROTOCOL_SENSOR_ID_IBAT           = 0x0A;
const PROTOCOL_SENSOR_ID_V3V3           = 0x0B;
const PROTOCOL_SENSOR_ID_I3V3           = 0x0C;
const PROTOCOL_SENSOR_ID_V5             = 0x0D;
const PROTOCOL_SENSOR_ID_I5             = 0x0E;

let cmdLength = [
    { id: PROTOCOL_CMD_ERROR,               recLength: 2,      sendLenth: 1}, // rec: CMD + ERROR CODE            send: CMD
    { id: PROTOCOL_CMD_GET_PARACHUTE,       recLength: 2,      sendLenth: 1}, // rec: CMD + PARACHUTE STATE       send: CMD
    { id: PROTOCOL_CMD_SET_PARACHUTE,       recLength: 2,      sendLenth: 2}, // rec: CMD + STATE                 send: CMD + SETTER
    { id: PROTOCOL_CMD_GET_BALLOON,         recLength: 2,      sendLenth: 1}, // rec: CMD + PARACHUTE STATE       send: CMD
    { id: PROTOCOL_CMD_SET_BALLOON,         recLength: 2,      sendLenth: 2}, // rec: CMD + STATE                 send: CMD + SETTER
    { id: PROTOCOL_CMD_GET_SENSOR,          recLength: 'variable',  sendLenth: 2}, // Explained below
    { id: PROTOCOL_CMD_GET_BATTERY,         recLength: 2,      sendLenth: 1} , // rec: CMD + BATTERY %             send: CMD
    { id: PROTOCOL_CMD_SET_REPORT_FREQ,     recLength: 2,      sendLenth: 2}, // rec: CMD + STATE                 send: CMD + SETTER
    { id: PROTOCOL_CMD_START_STOP_REPORT,   recLength: 2,      sendLenth: 2}, // rec: CMD + STATE                 send: CMD + SETTER
    { id: PROTOCOL_CMD_GET_GPS,             recLength: 9,      sendLenth: 1}  // rec: CMD + LAT MSB + LAT[2] + LAT[1] + LAT LSB + LNG MSB + LNG[2] + LNG[1] + LNG LSB    send: CMD
];

let sensorsLength = [
    {id: PROTOCOL_SENSOR_ID_GYROSCOPE,      recLength: 8, sendLenth: 2}, // rec: CMD + ID + MSB x + LSB x + MSB y + LSB y + MSB z + LSB z   send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_MAGNETOMETER,   recLength: 8, sendLenth: 2}, // rec: CMD + ID + MSB x + LSB x + MSB y + LSB y + MSB z + LSB z   send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_ACCELEROMETER,  recLength: 8, sendLenth: 2}, // rec: CMD + ID + MSB x + LSB x + MSB y + LSB y + MSB z + LSB z   send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_ORIENTATION,    recLength: 8, sendLenth: 2}, // rec: CMD + ID + MSB x + LSB x + MSB y + LSB y + MSB z + LSB z   send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_TEMPERATURE,     recLength: 3, sendLenth: 2}, // rec: CMD + ID + TEMP °              send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_HUMIDITY,       recLength: 3, sendLenth: 2}, // rec: CMD + ID + HUM %               send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_PRESSURE,       recLength: 4, sendLenth: 2}, // rec: CMD + ID + MSB p + LSB p       send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_ALTITUDE,       recLength: 4, sendLenth: 2}, // rec: CMD + ID + MSB a + LSB a       send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_VBAT,           recLength: 4, sendLenth: 2}, // rec: CMD + ID + MSB mV + LSB mV     send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_IBAT,           recLength: 4, sendLenth: 2}, // rec: CMD + ID + MSB mA + LSB mA     send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_V3V3,           recLength: 4, sendLenth: 2}, // rec: CMD + ID + MSB mV + LSB mV     send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_I3V3,           recLength: 4, sendLenth: 2}, // rec: CMD + ID + MSB mA + LSB mA     send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_V5,             recLength: 4, sendLenth: 2}, // rec: CMD + ID + MSB mV + LSB mV     send: CMD + ID 
    {id: PROTOCOL_SENSOR_ID_I5,             recLength: 4, sendLenth: 2}  // rec: CMD + ID + MSB mA + LSB mA     send: CMD + ID 
]

let cmd_decoded = {
    cmd: 0, 
    length: 0,
    error: 0,  
    data: {} 
};

/**
 * Clear cmd_decoded
 */
protocol_clear_decoded = function(){
    cmd_decoded.cmd = 0;
    cmd_decoded.length = 0;
    cmd_decoded.error = 0;  
    cmd_decoded.data = {}; 
}

/**
 * Calculate checksum of data
 * @param {*} data array of data to calculate checksum
 * @returns value of checksum 
 */
protocol_calculate_checksum = function(data){
    let checksum = 0;
    for(let n = 0; n < data.length; n++){
        checksum += data[n];
    }
    return 0xff - (checksum & 0xff);
}

/**
 * Validate package cheksum 
 * @returns true if checksum is ok
 */
protocol_validate_checksum = function(packet){
    let checksum = 0;
    let initFromByte = 3;

    for(let d= initFromByte ;d < packet.length; d++){
        checksum +=  packet[d];
    }
    checksum = checksum & 0xff;

    if(checksum  == 0xff){
        return true;
    }
    return false;
}

/**
 * Validate package length of received packet
 * @returns true if length is ok
 */
protocol_validate_rec_length = function(packet){
    let pkgLength = cmd_decoded.length;
    let cmd= cmd_decoded.cmd;
    let length;
    if(cmd == PROTOCOL_CMD_GET_SENSOR){
        let sensorID = packet[4];
        // length = sensorsLength[sensorID-1].recLength;
        let validID = sensorsLength.filter(function(n) {
            return (n.id == sensorID)
        });
        
        if(validID[0] && validID[0].recLength ==  pkgLength){
            return true;
        }     
    }else{
        length = cmdLength[cmd].recLength;
        if(length == pkgLength){
            return true;
        }   
    }
    console.log(cmd,pkgLength,length)
    return false; 
}

/**
 * Add escape to packet
 * @param {*} packet to modify
 * @returns valid packet
 */
protocol_add_escape = function(packet){
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
protocol_remove_escape = function(packet){
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
 * @param {*} packet array to decode
 * @returns object cmd_decoded
 */
protocol_decode_data = function(packet)
{
    let cmd = cmd_decoded.cmd;
    let length = cmd_decoded.length;
    switch(cmd){
        case PROTOCOL_CMD_ERROR:
        case PROTOCOL_CMD_GET_PARACHUTE:
        case PROTOCOL_CMD_SET_PARACHUTE:
        case PROTOCOL_CMD_GET_BALLOON:
        case PROTOCOL_CMD_SET_BALLOON:
        case PROTOCOL_CMD_GET_BATTERY:
        case PROTOCOL_CMD_SET_REPORT_FREQ:
            cmd_decoded.data.state = packet[4]
            cmd_decoded.error = CMD_OK;   
            break;
        case PROTOCOL_CMD_GET_SENSOR:
            let id = packet[4];
            cmd_decoded.data.sensorID = id;
            switch(id){
                case PROTOCOL_SENSOR_ID_GYROSCOPE:
                case PROTOCOL_SENSOR_ID_MAGNETOMETER:
                case PROTOCOL_SENSOR_ID_ACCELEROMETER:
                case PROTOCOL_SENSOR_ID_ORIENTATION:
                    cmd_decoded.data.x = protocol_convert_to_signed(packet[5] * Math.pow(2,8) + packet[6]);
                    cmd_decoded.data.y = protocol_convert_to_signed(packet[7] * Math.pow(2,8) + packet[8]);
                    cmd_decoded.data.z = protocol_convert_to_signed(packet[9] * Math.pow(2,8) + packet[10]);
                    break;
                case PROTOCOL_SENSOR_ID_TEMPERATURE:
                case PROTOCOL_SENSOR_ID_HUMIDITY:
                    cmd_decoded.data.value = packet[5];
                    break;
                case PROTOCOL_SENSOR_ID_PRESSURE:
                case PROTOCOL_SENSOR_ID_ALTITUDE:
                case PROTOCOL_SENSOR_ID_VBAT:
                case PROTOCOL_SENSOR_ID_IBAT:
                case PROTOCOL_SENSOR_ID_V3V3:
                case PROTOCOL_SENSOR_ID_I3V3:
                case PROTOCOL_SENSOR_ID_V5:
                case PROTOCOL_SENSOR_ID_I5:
                    cmd_decoded.data.value = packet[5] * Math.pow(2,8) + packet[6];
                    break;
                default: // Sensors added by user
                    cmd_decoded.data.value = packet[5] * Math.pow(2,8) + packet[6];
            }
            cmd_decoded.error = CMD_OK; 
            break; 
        case PROTOCOL_CMD_GET_GPS:
            // To ensure cross-platform compatibility when sending the latitude and longitude values
            //  we convert them to integers instead of sending them in floating point format.
            // The equation to convert the angle to integer is:
            //      int encodedAngle = (int)(angle * (0x7FFFFFFF / 180.0));
            // The equation to convert from integer to angle is:
            //      float angle = (encodedAngle / (0x7FFFFFFF / 180.0));
            // See: https://stackoverflow.com/questions/7934623/what-is-the-approximate-resolution-of-a-single-precision-floating-point-number-w

            let latInt = packet[4] * Math.pow(2,24) + packet[5] * Math.pow(2,16) + packet[6] * Math.pow(2,8) + packet[7];
            let lngInt = packet[8] * Math.pow(2,24) + packet[9] * Math.pow(2,16) + packet[10] * Math.pow(2,8) + packet[11];

            cmd_decoded.data.lat = ((~~latInt) / (2147483647 / 180.0));
            cmd_decoded.data.lng = ((~~lngInt) / (0x7FFFFFFF / 180.0));
            
            console.log(latInt.toString(16));

            break;  
        default:
            cmd_decoded.error = CMD_ERROR_WRONG_FORMAT;                           
    }
    return cmd_decoded
}

/**
 * Format packet
 * @param {*} packet to format
 * @returns formatted packet
 */
protocol_format_packet = function(packet){
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
 * Format packet
 * @param {*} packet to format
 * @returns formatted packet
 */
protocol_convert_to_signed = function(value){
    if ((value & 0x8000) > 0) {
        value = value - 0x10000;
    }
    return value;
}

module.exports =
{

    /**
     * Create packet 
     * @param {*} cmd to parse and create valid packet
     * @param {*} data to format
     * @returns packet with valid protocol
     */
    create_packet: function(cmd, data)
    {
        let packet = [];
        switch(cmd){
            case PROTOCOL_CMD_ERROR:
            case PROTOCOL_CMD_GET_PARACHUTE:
            case PROTOCOL_CMD_SET_PARACHUTE:
            case PROTOCOL_CMD_GET_BALLOON:
            case PROTOCOL_CMD_SET_BALLOON:
            case PROTOCOL_CMD_GET_BATTERY:
            case PROTOCOL_CMD_SET_REPORT_FREQ:
                cmd_decoded.data.state = packet[4]
                cmd_decoded.error = CMD_OK;   
                break;
            case PROTOCOL_CMD_GET_SENSOR:
                let id = packet[4];
                cmd_decoded.data.sensorID = id;
                switch(id){
                    case PROTOCOL_SENSOR_ID_GYROSCOPE:
                    case PROTOCOL_SENSOR_ID_MAGNETOMETER:
                    case PROTOCOL_SENSOR_ID_ACCELEROMETER:
                    case PROTOCOL_SENSOR_ID_ORIENTATION:
                        cmd_decoded.data.x = packet[5] * Math.pow(2,8) + packet[6];
                        cmd_decoded.data.y = packet[7] * Math.pow(2,8) + packet[8];
                        cmd_decoded.data.z = packet[9] * Math.pow(2,8) + packet[10];
                       console.log(cmd_decoded.data.x);
                        break;
                    case PROTOCOL_SENSOR_ID_TEMPERATURE:
                    case PROTOCOL_SENSOR_ID_HUMIDITY:
                        cmd_decoded.data.value = packet[5];
                        break;
                    case PROTOCOL_SENSOR_ID_PRESSURE:
                    case PROTOCOL_SENSOR_ID_ALTITUDE:
                    case PROTOCOL_SENSOR_ID_VBAT:
                    case PROTOCOL_SENSOR_ID_IBAT:
                    case PROTOCOL_SENSOR_ID_V3V3:
                    case PROTOCOL_SENSOR_ID_I3V3:
                    case PROTOCOL_SENSOR_ID_V5:
                    case PROTOCOL_SENSOR_ID_I5:
                        cmd_decoded.data.value = packet[5] * Math.pow(2,8) + packet[6];
                        break;
                    default: // Sensors added by user
                        cmd_decoded.data.value = packet[5] * Math.pow(2,8) + packet[6];
                }
                cmd_decoded.error = CMD_OK; 
                break; 
            case PROTOCOL_CMD_GET_GPS:
                // To ensure cross-platform compatibility when sending the latitude and longitude values
                //  we convert them to integers instead of sending them in floating point format.
                // The equation to convert the angle to integer is:
                //      int encodedAngle = (int)(angle * (0x7FFFFFFF / 180.0));
                // The equation to convert from integer to angle is:
                //      float angle = (encodedAngle / (0x7FFFFFFF / 180.0));
                // See: https://stackoverflow.com/questions/7934623/what-is-the-approximate-resolution-of-a-single-precision-floating-point-number-w
    
                let latInt = packet[5] * Math.pow(2,32) + packet[6] * Math.pow(2,16) + packet[7] * Math.pow(2,8) + packet[8];
                let lngInt = packet[9] * Math.pow(2,32) + packet[10] * Math.pow(2,16) + packet[11] * Math.pow(2,8) + packet[12];
    
                cmd_decoded.data.lat = (latInt / (0x7FFFFFFF / 180.0));
                cmd_decoded.data.lng = (lngInt / (0x7FFFFFFF / 180.0));
    
                break;  
            default:
                cmd_decoded.error = CMD_ERROR_WRONG_FORMAT;                           
        }


        return protocol_format_packet(packet);
    },

    /**
     * Create packet 
     * @param {*} data array to format where data[0] is cmd 
     * @returns packet with valid protocol
     */
    format_packet: function(data)
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
    },

    /**
     * Decode data from packet
     * @param {*} packet to decode
     * @returns Object with decoding data
     */
    decode_packet: function(packet)
    {
        let pkg = protocol_remove_escape(packet.data);

        protocol_clear_decoded()
        cmd_decoded.cmd = pkg[3];
        cmd_decoded.length = pkg[1] * 64 + pkg[2];

        

        if(pkg[0] != START_BYTE){
            cmd_decoded.error = CMD_ERROR_WRONG_FORMAT;
            return cmd_decoded;
        }

        let validID = cmdLength.filter(function(n) {
            return (n.id == cmd_decoded.cmd)
        });

        if(validID.length != 1){
            cmd_decoded.error = CMD_ERROR_WRONG_FORMAT; 
            return cmd_decoded;
        }

        if(!protocol_validate_rec_length(pkg)){
            cmd_decoded.error = CMD_ERROR_LENGTH;
            return cmd_decoded;
        }

        if(!protocol_validate_checksum(pkg)){
            cmd_decoded.error = CMD_ERROR_CHECKSUM;
            return cmd_decoded;
        }
 
        return protocol_decode_data(pkg);
    },
}