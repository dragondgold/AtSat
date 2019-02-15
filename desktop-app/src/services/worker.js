
const path = require('path');
const cc1101 = require('./cc1101.js');
//const pathMCP2210CLI = path.resolve('./../../build/MCP2210/mcp2210-cli.exe');
const protocol = require('./protocol.js');
const pathMCP2210CLI = path.resolve('./build/MCP2210/mcp2210-cli.exe');

// Init the CC1101 and set RX mode
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
