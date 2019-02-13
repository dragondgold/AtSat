const path = require('path');
const cc1101 = require('./cc1101.js');
let pathMCP2210CLI = path.resolve('./../../build/MCP2210/mcp2210-cli.exe');

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
    console.log(Date.now() + ": Data -> " + JSON.stringify(cc1101.cc1101_read_data()));
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
let count = 0;
setInterval(function()
{
  console.log(Date.now() + ": Send " + (count++));
  cc1101.cc1101_send_data([0x7E, 0x00, 0x02, 0x05, 0x01, 0xF9]);
}, 1000);
