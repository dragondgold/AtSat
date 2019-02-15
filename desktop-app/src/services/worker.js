
const path = require('path');
const cc1101 = require('./cc1101.js');
const pathMCP2210CLI = path.resolve('./../../build/MCP2210/mcp2210-cli.exe');
const protocol = require('./protocol.js');
//const pathMCP2210CLI = path.resolve('./build/MCP2210/mcp2210-cli.exe');

// Init the CC1101 and set RX mode

/*
let data = {
  //data: [ 0x7e, 0 , 2 , 0,0, 255]
  data: [ 0, 0x7e, 0 , 23, 5,  8,0,0,0,0,0,0, 1,0,0, 3,0,0, 7, 0,0,0,0,0,0,0,0, 231]
  //data: [ 0x7d, 0, 0, 0, 0x7e, 0 , 8, 5,  8,0,0,0,0,0,0,242]
}

let decoded = protocol.decode_packet(data);
console.log(Date.now() + ": Data -> " + JSON.stringify(decoded));
console.log(JSON.stringify(decoded.decoded));
*/

const cmd =
[
    { name: 'getError', data: [] },         
    { name: 'getParachute', data: []},    
    { name: 'setParachute', data: [1] },    
    { name: 'getBalloon', data:  [] },      
    { name: 'setBalloon', data: [1] },          
    { name: 'getSensor', data: [ 1,2,3]},
    { name: 'getBattery', data: [] },      
    { name: 'setReport', data: [2] },  
    { name: 'startReport', data: [1] }      
]

for(let a = 0; a < cmd.length; a++){
  let packet = protocol.create_packet( cmd[a].name, cmd[a].data );
  console.log(JSON.stringify(packet));
}


/*
console.log("Init: " + cc1101.cc1101_init(pathMCP2210CLI));

cc1101.cc1101_set_cli_path(pathMCP2210CLI);
cc1101.cc1101_set_rx(true);

// Check for incoming data every 50 ms
setInterval(function()
{
  let bytes = cc1101.cc1101_bytes_in_rx_fifo();
  console.log(Date.now() + ": Bytes: " + bytes);
  //console.log(cc1101.cc1101_read_status(0x35));

  // Data available?
  if(bytes > 0 && cc1101.cc1101_is_packet_sent_available())
  {
    let data = cc1101.cc1101_read_data();
    console.log(Date.now() + ": Data -> " + JSON.stringify(data));
    let decoded = protocol.decode_packet(data);
    console.log(Date.now() + ": Data -> " + JSON.stringify(decoded));
    //process.send(decoded)
    cc1101.cc1101_set_rx(true);
  }

  // Flush the RX FIFO if needed
  if(cc1101.cc1101_is_rx_overflow())
  {
      console.log(Date.now() + ": Flush");
      cc1101.cc1101_set_rx(true);
  }
}, 50);

// Send data every 1000 ms
let a
setInterval(function()
{
  cc1101.cc1101_send_data([0x7E, 0x00, 0x02, 5,3, 247]);
  
  if(a ==1){
    cc1101.cc1101_send_data([0x7E, 0x00, 0x2,4,1, 250]);
    a = 0;
  }else{
    cc1101.cc1101_send_data([0x7E, 0x00, 0x2,4,0, 251]);
    a = 1;
  }

}, 100);
*/