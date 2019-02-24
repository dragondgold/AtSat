const cc1101 = require('./cc1101.js');
const protocol = require('./protocol.js');
const SerialPort = require('serialport');

const maxAttempt = 2;
const vid = 0x4D8;
const pid = 0xDE;
const MCP_MANUFACTURER = 'Microchip Technology, Inc.';

let control = {
  et:{
    attempt: 0,
    connected: false,
    error: 0,
    intervalConnect: {}
  },
  cansat:{
    attempt: 0,
    timeout: 100,
    connected: false,
    missionActive: false,
    intervalMission: {}
  }
};

let isCCUBusy =false;
let queueSend = [];
let comName = '';

/**
 * Decode data from packet
 * Listen for msg from parent and sent it to cc1101
 */
if(process.on)
{
  process.on('message', (msg) => 
  {
    cmdToSendlistener(msg);
    cmdWorker(msg);
  });
}

let cmdToSendlistener =  function(msg)
{
  if(msg.cmdToSend)
  {
    if(control.cansat.connected || true){
      let packet = protocol.create_packet( msg.cmdToSend.cmd, msg.cmdToSend.valuesArray );

      if(process.send)process.send({queueAdd: packet})

      // Add cmd to queue
      console.log('Pushing to queue: ' + packet);
      queueSend.push(packet);
    }
  }
}

let cmdWorker = function(msg)
{
  if(msg.cmdToWorker)
  {
    let cmd =msg.cmdToWorker.cmd;
    if(cmd == 'startReport')
    {
      control.cansat.missionActive = true;
    
    }

    if(cmd == 'endReport')
    {
      control.cansat.missionActive = false;
    }

    if(cmd == 'connectSerialPort')
    {
      comName = msg.cmdToWorker.comName;
    }

    if(cmd == 'disconnectSerialPort')
    {
      comName = '';
      clearInterval(control.et.intervalConnect);
      clearInterval(control.cansat.intervalMission);
      control.et.connected = false;
      control.cansat.connected = false;
      let port = cc1101.get_port();
      port.close();
    }

  }
}

/**
 * Send command to CANSAT 
 */
let sendCMDCanSat= async function(){
  if(!isCCUBusy){
    isCCUBusy = true;
    console.log('Transmiting');

    if(queueSend.length > 0){
      console.log('Packets in queue: ' + JSON.stringify(queueSend) );
      let packet = queueSend.shift();
      
      console.log('Trying to send: ' + packet);
      console.log('Packets in queue before shift: ' + JSON.stringify(queueSend) );
      //process.send({packet})
      if(packet.length > 0){       
        await cc1101.cc1101_send_data(packet);     
        if(process.send) process.send({queueSend: packet});
      }
    }
    isCCUBusy = false;
    console.log('END Transmiting');
  }
}

/**
 * Send command to cc1101 
 * @param {*} cmdName name of cmd define in protocol.js
 * @param {*} data define length in protocol.js
 * @param {*} sendToparent if we need send response to parent
 * @returns true if we received a valid packet and sent it to parent
 */
let sendCommand = function(cmdName, data, sendToparent){

  let packet = protocol.create_packet( cmdName, data );
  if(process.send){
    process.send(packet)
  }else{

  }

  console.log('Pushing to queue: ' + packet);
  queueSend.push(packet);
}

/**
 * Wait for response from cc1101 with a timeout
 * @param {*} sendToparent if we need send response to parent
 * @returns true if we received a valid packet and sent it to parent
 */
let checkForData = async function(sendToparent){
  if(isCCUBusy){
    return;
  }
  isCCUBusy = true;
  console.log('Receiving');
  await cc1101.cc1101_bytes_in_rx_fifo().then( async function(bytes)
  {
    if(bytes > 0)
    {
      console.log("Bytes: " + bytes);
      let data = await cc1101.cc1101_read_data();
      let decoded = protocol.decode_packet(data);

      if(data.valid){
        console.log(decoded.decoded);

        if(process.send && sendToparent){
          process.send({
            newCMDReceived: {
              data: decoded,
              packet: data
            }
          })
        }else{
          process.send({
            newCMDReceived: {
              packet: data
            }
          })
        }
      }
      
      
      if(bytes & 0x80)
      {
        await cc1101.cc1101_set_rx(true);
      }
    }
    isCCUBusy = false;
    console.log('END Receiving');

  }).catch(function()
  { 
    isCCUBusy = false;
    console.log('END Receiving');
  });
}


let missionActive = async function ()
{
  // To avoid queue full of samples
  if(queueSend.length < 3){
    if(control.cansat.missionActive){
      getPowerSupplies();
      getSensors();
    }
    getBatteryLevel();
  }  
}

/**
 * Get all samples from sensors
 */
let getSensors = function ()
{
  return sendCommand('getSensor', [7,8,9,10,12,13,14,15],true); 
}

/**
 * Get all samples from power supplies
 */
let getPowerSupplies = function ()
{
  sendCommand('getSensor', [1,2,3,4,5,6], true); 
}

/**
 * Get battery level
 */
let getBatteryLevel = function ()
{
  sendCommand('getBattery', [], true); 
}


/**
 * Check if cansat is connected
 */
let getCansatConnected = function(){
  sendCommand('getBattery', [], true); 
}

/**
 *  Logic to connected to an ET
 */
let checkForETConnected = async function()
{
  let detectedDev = false;
  if(usb == undefined){
    detectedDev = true;
  }else{
    detectedDev = usb.findByIds( vid, pid ) 
  }
  if( detectedDev  && control.et.error == 0)
  {
    if(!control.et.connected)
    {
      if(process.send)
      {
        process.send({
          et: {
            state: 'config'
          }
        });
      }
  
      while(control.et.attempt < maxAttempt && !control.et.connected)
      {
        control.et.connected = await cc1101.cc1101_init('COM12');
        if(control.et.connected)
        {
          control.et.attempt = 0; 
          
        }
        else
        {
          control.et.attempt ++;
          console.log( "Trying to connect to an ET, Attempt: " + control.et.attempt)
        }
      }  
      control.et.attempt = 0;



      if(control.et.attempt == maxAttempt && !control.et.connected)
      {
        control.cansat.connected = false;
        console.log( "Error configuring ET");
        if(process.send)
        {
          control.et.error = -1;
          process.send({
            et: {
              state: 'error'
            }
          })
        }
        return false
      }
      else if(control.et.connected)
      {
        //control.cansat.interval = setInterval(intervalConnectCanSat, control.cansat.time);
        console.log( "ET CONNECTED: " + control.et.connected);
        if(process.send)
        {
          control.et.error = 0;
          process.send({
            et: {
              state: 'connected'
            }
          })
          console.log("ET CONNECTED")
          return true
        }
      }      
    }else{
      return true;
    }    
  }
  else if(control.et.connected)
  {
    if(process.send)
    {
      control.et.connected = false;
      control.et.cansat = false;
      process.send({
        et: {
          state: 'disconnected'
        }
      });
    }
    return false
  }
}


let findET = function(){
  if(!control.et.connected)
  {
    SerialPort.list().then( function(ports){
      let serialPorts = [];
      for(let sp = 0; sp < ports.length ; sp++){
        if( ports[sp].manufacturer == MCP_MANUFACTURER)
        {
          serialPorts.push( ports[sp].comName)
        }
      }
      if(process.send) process.send({serialPorts: serialPorts}); 
    })

    if(comName != ''){
      process.send({
        et: {
          state: 'config'
        }
      });
      cc1101.cc1101_init(comName).then(async function()
      {

        control.et.connected = true;
        process.send({
          et: {
            state: 'connected'
          }
        });
        control.et.intervalConnect = setInterval(async function(){
          await checkForData(true);
          await sendCMDCanSat();
        }, 200); 

        control.cansat.intervalMission =setInterval(async function(){
          missionActive();
        }, 1000); 
      });
    }

  }
  else{

  }
}

/*

*/ 

setInterval(async function(){
  findET();
}, 1000); 
