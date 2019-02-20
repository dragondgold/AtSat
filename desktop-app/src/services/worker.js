const path = require('path');
const cc1101 = require('./cc1101.js');
const protocol = require('./protocol.js');

let usb;

let pathToCLI;
if(process.send == undefined){
  pathToCLI = path.resolve('./../../build/MCP2210/mcp2210-cli.exe');
}else{
  usb = require('usb');
  pathToCLI = path.resolve('./build/MCP2210/mcp2210-cli.exe');
}

const maxAttempt = 2;
const vid = 0x4D8;
const pid = 0xDE;

let control = {
  et:{
    attempt: 0,
    connected: false,
    error: 0
  },
  cansat:{
    attempt: 0,
    timeout: 100,
    connected: false,
    missionActive: false
  }
};

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
    console.log(packet)
  }

  if(packet.length > 0){
    cc1101.cc1101_send_data(packet);
    if(waitResponse(sendToparent)){
      console.log("Response received");
      return true;
    }else{
      console.log("No response");
      return false;
    }
  }
}

/**
 * Wait for response from cc1101 with a timeout
 * @param {*} sendToparent if we need send response to parent
 * @returns true if we received a valid packet and sent it to parent
 */
let waitResponse = function(sendToparent){
  let packetReceived = false;
  var end = new Date().getTime() + control.cansat.timeout;

  while(!packetReceived &&  new Date().getTime() < end)
  {
    let bytes = cc1101.cc1101_bytes_in_rx_fifo();
  
    // Data available?
    if(bytes > 0 && cc1101.cc1101_is_packet_sent_available())
    {
      let data = cc1101.cc1101_read_data();
      console.log(Date.now() + ": Data -> " + JSON.stringify(data));
      
      if(data.valid){
        let decoded = protocol.decode_packet(data);
        console.log(Date.now() + ": Data -> " + JSON.stringify(decoded));
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
        cc1101.cc1101_set_rx(true);
        packetReceived = true
      }
      
    }
    // Flush the RX FIFO if needed
    if(bytes & 0x80)
    {
        console.log(Date.now() + ": Flush");
        cc1101.cc1101_set_rx(true);
    }
  }
  console.log("Response received: " + packetReceived);
  return packetReceived;
}


/**
 * Get all samples from sensors
 * @returns true if we received a valid packet and sent it to parent. Use waitResponse() internally
 */
let getSensors = function ()
{
  return sendCommand('getSensor', [7,8,9,10,12,13,14],true); 
}

/**
 * Get all samples from power supplies
 * @returns true if we received a valid packet and sent it to parent. Use waitResponse() internally
 */
let getPowerSupplies = function ()
{
  return sendCommand('getSensor', [1,2,3,4,5,6], true); 
}

/**
 * Get battery level
 * @returns true if we received a valid packet and sent it to parent. Use waitResponse() internally
 */
let getBatteryLevel = function ()
{
  return sendCommand('getBattery', [], true); 
}


/**
 * Check if cansat is connected
 * @returns true if we received a valid packet and sent it to parent. Use waitResponse() internally
 */
let getCansatConnected =function(){
  return sendCommand('getBattery', [], true); 
}

/**
 *  Logic to connected to an ET
 */
let checkForETConnected = function()
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
        control.et.connected = cc1101.cc1101_init(pathToCLI);
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


/**
 * Decode data from packet
 * Listen for msg from parent and sent it to cc1101
 */
if(process.on)
{
  process.on('message', (msg) => 
  {
    cmdToSendlistener(msg)
  });
}

let cmdToSendlistener = function(msg)
{
  if(msg.cmdToSend)
  {
    process.send(msg)
    if(control.cansat.connected || true){
      let packet = protocol.create_packet( msg.cmdToSend.cmd, msg.cmdToSend.valuesArray );

      if(msg.cmdToSend.cmd == 'startReport'){
        control.cansat.missionActive = msg.cmdToSend.valuesArray[0];
      }

      process.send(packet)
      if(packet.length > 0){
        cc1101.cc1101_send_data(packet);
      }
    }
  }
}



/**
 * Check for connected ET and Cansat. If the mission is active we send cmd to get sensor samples.
 *  Work in an interval
 */
let main = function(){
  try {
    if( checkForETConnected() )
    {
      if(control.cansat.missionActive && control.cansat.connected)
      {
        if( getPowerSupplies() )
        {
          control.cansat.connected = true;
        }
        else
        {
          control.cansat.connected = false;
        }
        
        if( getSensors() )
        {
          control.cansat.connected = true;
        }
        else 
        {
          control.cansat.connected = false;
        }
      }
      else
      {
        if( getCansatConnected() )
        {
          
          if(!control.cansat.connected && process.send){
            process.send({
              cansat: {
                connected: true
              }
            })
          }
          console.log("CANSAT CONNECTED");
          control.cansat.connected = true;
        }
        else 
        {
          if(control.cansat.connected && process.send){
            process.send({
              cansat: {
                connected: false
              }
            })
          }
          console.log("CANSAT DISCONNECTED");
          control.cansat.connected = false;
        }
  
      }
    }
  } catch (error) {
    if(process.send){
      process.send((error))
    }
  }
}
setInterval(main, 1000)