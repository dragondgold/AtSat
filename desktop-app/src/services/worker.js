const path = require('path');
const cc1101 = require('./cc1101.js');
let pathMCP2210CLI = path.resolve('./../../build/MCP2210/MCP2210CLI.exe');

console.log("Init: " + cc1101.cc1101_init(pathMCP2210CLI));
cc1101.cc1101_set_rx(true);

setInterval(function()
{
  let bytes = cc1101.cc1101_bytes_in_rx_fifo();
  if(bytes > 65)
  {
    console.log("Flush");
    cc1101.cc1101_set_rx(true);
  }
  console.log("Bytes: " + bytes);

  if(bytes > 0 && cc1101.cc1101_is_packet_sent_available())
  {
    console.log(cc1101.cc1101_read_data());
  }
}, 300);

/*
console.log("Init: " + cc1101.cc1101_init(pathMCP2210CLI));

cc1101.cc1101_set_cli_path(pathMCP2210CLI);
cc1101.cc1101_send_data([20, 52, 200]).then(function()
{
  console.log("Sent!");
})
.catch(function()
{
  console.log("FAIL Sent!");
});*/
