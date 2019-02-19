const path = require('path');
const cc1101 = require('./cc1101.js');
const protocol = require('./protocol.js');

let usb;

let pathToCLI = path.resolve('./../../build/MCP2210/mcp2210-cli.exe');

let init = false
while(!cc1101.cc1101_init(pathToCLI)){

}

console.log("Init: " + true);

cc1101.cc1101_set_cli_path(pathToCLI);
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

    if(data.valid){
        let decoded = protocol.decode_packet(data);
        console.log(Date.now() + ": Data -> " + JSON.stringify(decoded));
    }

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
  //cc1101.cc1101_send_data([0x7E, 0x00, 0x07, 5,3,1,2,4,6,7, 227]);
  cc1101.cc1101_send_data([0x7E, 0x00, 0x01, 1,254 ]);

}, 1000);