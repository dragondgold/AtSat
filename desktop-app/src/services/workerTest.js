const cc1101 = require('./cc1101.js');

cc1101.cc1101_init("COM3").then(function(r)
{
    console.log(r);
    console.time("servo");
    cc1101.cc1101_send_data([0x7e, 0,2, 2,1, 252]).then(function()
    {
      setInterval(function()
      {
        cc1101.cc1101_bytes_in_rx_fifo().then(function(bytes)
        {
          console.log("Bytes: " + bytes);
        });
      }, 100)

      console.timeEnd("servo");
      console.log("Sent!");
    })
    .catch(function(err)
    {
      console.log("Fail sent: " + err);
    });
})
.catch(function(e)
{
    console.error(e);
});

/*
var SerialPort = require("serialport");
var port = new SerialPort("COM3", {
      baudRate: 115200
});
const Readline = require('@serialport/parser-readline')
const parser = new Readline()

var buffer = new Buffer(5);
buffer[0] = 0x7E;
buffer[1] = 0x02;
buffer[2] = 0x01;
buffer[3] = 0x37;
buffer[4] = 0xC7;

function toHexString(byteArray) {
  return Array.from(byteArray, function(byte) {
    return ('0' + (byte & 0xFF).toString(16)).slice(-2);
  }).join(' ')
}

port.on("open", function () {
	console.log('Port opened!');
	
	setInterval(function()
	{
		console.time("send");
		port.write(buffer);
	}, 100);
});

// open errors will be emitted as an error event 
port.on('error', function(err) {
  console.log('Error -> ', err.message);
})

port.on('data', function(data)
{
	console.timeEnd("send");
  console.log("Received: " + toHexString(data));
});*/

/*
cc1101.cc1101_init();

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

}, 1000);*/