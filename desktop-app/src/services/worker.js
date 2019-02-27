try {

  let SerialPort;
  if (process.argv[2]) { // is isDevelopment
    SerialPort = require('serialport');
  } else {
    SerialPort = require('./../app/node_modules/serialport/lib/index.js');
  }

  console.log("Worker init");

  const cc1101 = require('./cc1101.js');
  const protocol = require('./protocol.js');

  const maxAttempt = 2;
  const vid = 0x4D8;
  const pid = 0xDE;
  const MCP_MANUFACTURER = 'Microchip Technology, Inc.';

  const powerSuppliesArray = [1, 2, 3, 4, 5, 6]
  const sensorsArray = [7, 8, 9, 10, 12, 13, 14, 15]

  //const SerialPort = process.argv[2]

  let control = {
    et: {
      attempt: 0,
      connected: false,
      error: 0,
      intervalConnect: {}
    },
    cansat: {
      attempt: 0,
      timeout: 100,
      connected: false,
      missionActive: false,
      intervalMission: {}
    }
  };

  let isCCUBusy = false;
  let queueSend = [];
  let comName = '';
  let comPorts = 0;

  /**
   * Decode data from packet
   * Listen for msg from parent and sent it to cc1101
   */
  if (process.on) {
    process.on('message', (msg) => {
      cmdToSendlistener(msg);
      cmdWorker(msg);
    });
  }

  let cmdToSendlistener = function (msg) {
    if (msg.cmdToSend) {
      if (control.cansat.connected || true) {
        let packet = protocol.create_packet(msg.cmdToSend.cmd, msg.cmdToSend.valuesArray);

        if (process.send) process.send({ queueAdd: packet })

        // Add cmd to queue
        console.log('Pushing to queue: ' + packet);
        queueSend.push(packet);
      }
    }
  }

  let cmdWorker = function (msg) {
    if (msg.cmdToWorker) {
      let cmd = msg.cmdToWorker.cmd;
      if (cmd == 'startReport') {
        control.cansat.missionActive = true;

      }

      if (cmd == 'endReport') {
        control.cansat.missionActive = false;
      }

      if (cmd == 'connectSerialPort') {
        console.log("Conectar en port: " + msg.cmdToWorker.comName);
        comName = msg.cmdToWorker.comName;
      }

      if (cmd == 'disconnectSerialPort') {
        console.log("Disconect cmd received");
        if (control.et.connected) {
          clearOnDisconnect();
        }
      }

      if (cmd == 'disconnectToCansat') {
        control.cansat.connected = false;
      }


    }
  }

  let clearOnDisconnect = function () {
    comName = '';
    clearInterval(control.et.intervalConnect);
    clearInterval(control.cansat.intervalMission);
    console.log("Clear on disconect");
    if (control.et.connected) {
      let port = cc1101.get_port();
      port.close();
      console.log("Closing serial port");
    }
    isCCUBusy = false;
    control.et.connected = false;
    control.cansat.connected = false;
    process.send({
      et: {
        state: 'disconnected'
      }
    });
  }


  /**
   * Send command to CANSAT 
   */
  let sendCMDCanSat = async function () {
    if (!isCCUBusy) {
      isCCUBusy = true;
      console.log('Transmiting');

      if (queueSend.length > 0) {
        console.log('Packets in queue: ' + JSON.stringify(queueSend));
        let packet = queueSend.shift();

        console.log('Trying to send: ' + packet);
        console.log('Packets in queue before shift: ' + JSON.stringify(queueSend));
        //process.send({packet})
        if (packet.length > 0) {
          await cc1101.cc1101_send_data(packet);
          if (process.send) process.send({ queueSend: packet });
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
  let sendCommand = function (cmdName, data, sendToparent) {

    let packet = protocol.create_packet(cmdName, data);
    if (process.send) {
      process.send(packet)
    } else {

    }

    console.log('Pushing to queue: ' + packet);
    queueSend.push(packet);
  }

  /**
   * Wait for response from cc1101 with a timeout
   * @param {*} sendToparent if we need send response to parent
   * @returns true if we received a valid packet and sent it to parent
   */
  let checkForData = async function (sendToparent) {
    if (isCCUBusy) {
      return;
    }
    isCCUBusy = true;
    console.log('Receiving');
    let bytes = await cc1101.cc1101_bytes_in_rx_fifo();

    if (bytes > 0) {
      console.log("Bytes: " + bytes);
      let data = await cc1101.cc1101_read_data();
      let decoded = protocol.decode_packet(data);

      if (data.valid) {

        console.log('DATA RECEIVED: ' + JSON.stringify(data));
        console.log(decoded.decoded);
        if (!control.cansat.connected) {
          control.cansat.connected = true;
        }
        if (decoded.cmdName == 'freefallDetected') {
          console.log('caida libre detectada');
        }

        if (process.send && sendToparent) {
          process.send({
            newCMDReceived: {
              data: decoded,
              packet: data
            }
          })
        } else {
          process.send({
            newCMDReceived: {
              packet: data
            }
          })
        }
      }


      if (bytes & 0x80) {
        await cc1101.cc1101_set_rx(true);
      }
    }
    isCCUBusy = false;
    console.log('END Receiving');
  }


  let missionActive = async function () {
    // To avoid queue full of samples
    if (queueSend.length < 3) {
      if (control.cansat.missionActive) {
        getPowerSupplies();
        getSensors();
      }
      if (control.cansat.connected) {
        getBatteryLevel();
        getPowerSupplies();
        getSensors();
      }
    }
  }

  /**
   * Get all samples from sensors
   */
  let getSensors = function () {
    return sendCommand('getSensor', sensorsArray, true);
  }

  /**
   * Get all samples from power supplies
   */
  let getPowerSupplies = function () {
    sendCommand('getSensor', powerSuppliesArray, true);
  }

  /**
   * Get battery level
   */
  let getBatteryLevel = function () {
    sendCommand('getBattery', [], true);
  }


  /**
   * Check if cansat is connected
   */
  let getCansatConnected = function () {
    sendCommand('getBattery', [], true);
  }

  let main = async function () {
    await SerialPort.list().then(function (ports) {
      let serialPorts = [];
      for (let sp = 0; sp < ports.length; sp++) {
        if (ports[sp].manufacturer == MCP_MANUFACTURER) {
          serialPorts.push(ports[sp].comName)
        }
      }
      if (process.send) process.send({ serialPorts: serialPorts });

      if (comPorts.length != serialPorts.length) {
        comPorts = serialPorts;
        if (serialPorts.length == 0 && control.et.connected) {

          console.log('No serial port detected');
          clearOnDisconnect();
        }
      }
    })
    if (!control.et.connected) {
      if (comName != '') {
        process.send({
          et: {
            state: 'config'
          }
        });

        try {
          //clearOnDisconnect();
        } catch (error) {

        }

        console.log('INTENTANDO CONECTAR')
        await cc1101.cc1101_init(comName).then(async function () {
          isCCUBusy = false;
          control.et.connected = true;
          process.send({
            et: {
              state: 'connected'
            }
          });
          clearInterval(control.et.intervalConnect);
          control.et.intervalConnect = setInterval(async function () {
            await checkForData(true);
            await sendCMDCanSat();
          }, 200);

          clearInterval(control.cansat.intervalMission);
          control.cansat.intervalMission = setInterval(async function () {
            missionActive();
          }, 500);
        }).catch(function (e) {
          console.log("Error on cc1101 init: " + e);
          clearOnDisconnect();
        })
      }
    }
    else {

    }
  }

  setInterval(async function () {
    main();
  }, 1000);

} catch (error) {
  if (process.send) process.send({ test: 'ERROR: ' + error })
}