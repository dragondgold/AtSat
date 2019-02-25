const { fork } = require('child_process');
const ms = 700;

import store from '../store'
import utils from 'services/utils'
import defaultSensors from 'data/Sensors.js'
import { debug } from 'electron-log';

let worker = undefined

let defaultCmds = undefined

let queueSend = [];
let queueRec = [];
let waitingResponse = false;

export default {
    initWorker()
    {
        
        worker =  fork(require.resolve('./worker.js'));

        let that = this
        worker.on('message', (msg) => {
            //console.log(JSON.stringify(msg))
            that.etConnected(msg);
            that.cansatConnected(msg);
            that.newCMDReceived(msg);

            if(msg.test){
                console.log(msg.test);
            }

            if(msg.queueAdd || msg.queueSend){
                console.log(msg);
                /*
                if(msg.queueSend && queueSend.length != 0 && waitingResponse){
                    let last = queueSend.length -1 
                    queueSend[last].timeout = setTimeout(queueSend[last].reject,ms)
                }*/
            }

            if(msg.serialPorts){
                if(store.getters.axtec.serialPorts.length != msg.serialPorts.length){
                    store.commit('setSerialPorts', {ports: msg.serialPorts})   
                }
            }

            if(msg.newCMDReceived && msg.newCMDReceived.packet){
                let packet = msg.newCMDReceived.packet
                if(packet.rssi){
                    store.commit('setRSSI', { rssi: packet.rssi })   
                    //console.log("RSSI: "+ packet.rssi)  
                }
                if(packet.lqi){
                    store.commit('setLQI', { lqi: packet.lqi }) 
                    //console.log("LQI: "+ packet.lqi)      
                }
            }
        });

        worker.on('error', (err) => {
            debugger
            console.log("\n\t\tERROR: spawn failed! (" + err + ")")
        });

        worker.on('exit', (e) => {
            debugger
            console.log("Exit fork: " + e)
            worker = undefined
            setTimeout(that.initWorker, 5000);
        });
    },
    initDefaultStructure(){
        // Load sensors structure
        let sensors = defaultSensors.getSensors()
        for(let s = 0; s < sensors.length; s++){
            store.commit('addNewSensor', sensors[s])     
        }     
    },
    killWorker()
    {
        if(worker != undefined){
            worker.kill()
        }
    },
    etConnected(msg)
    {
        if(msg.et)
        {
            if(msg.et.state == 'error'){
                store.commit('pushNotificationModal',{ 
                    'title': vm.$t('cansat.notifications.etConfig'), 
                    'content': m.$t('cansat.notifications.etError'), 
                    'date': utils.getDate(),
                    'code': 0,
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.error')
                })
            }
            else if(msg.et.state == 'config'){
                store.commit('pushNotificationToast',{ 
                    'text': vm.$t('cansat.notifications.etConfig'), 
                    'icon': 'fa-wrench'          
                })
                store.commit('pushNotificationModal',{ 
                    'title': vm.$t('cansat.notifications.etConfig'), 
                    'date': utils.getDate(),
                    'code': 0,
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.info')
                })
            }
            else if( msg.et.state == 'connected' ){
                store.commit('pushNotificationToast',{ 
                    'text': vm.$t('cansat.notifications.etReady'), 
                    'icon': 'fa-check'          
                })
                store.commit('pushNotificationModal',{ 
                    'title': vm.$t('cansat.notifications.etReady'), 
                    'date': utils.getDate(),
                    'code': 0,
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.info')
                })
                store.commit('setStatusEarthStation', true)

            }else if(msg.et.state == 'disconnected'){
                store.commit('pushNotificationToast',{ 
                    'text': vm.$t('cansat.notifications.etDisconnected'), 
                    'icon': 'fa-unlink'          
                })
                store.commit('pushNotificationModal',{ 
                    'title': vm.$t('cansat.notifications.etDisconnected'), 
                    'date': utils.getDate(),
                    'code': 0,
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.info')
                })
                store.commit('setStatusEarthStation', false)
            }
        }
    },
    cansatConnected(msg)
    {
        if(msg.cansat)
        {
            if(msg.cansat.connected  && msg.cansat.connected != store.getters.axtec.project.cansat[0].connected )
            {
                store.commit('pushNotificationToast',{ 
                    'text': vm.$t('cansat.notifications.etCansatConnected'), 
                    'icon': 'fa-check'          
                })
                store.commit('setStatusCanSat', {connected: true, index: 0})
            }else if(!msg.cansat.connected  && msg.cansat.connected != store.getters.axtec.project.cansat[0].connected )
            {
                store.commit('pushNotificationToast',{ 
                    'text': vm.$t('cansat.notifications.etCansatDisconnected'), 
                    'icon': 'fa-unlink'          
                })
                store.commit('setStatusCanSat', {connected: false, index: 0})
            }
        }
    },
    sendCMDToWorker(cmdName, args, resolve,reject){
        worker.send( { 
            cmdToSend:{
                cmd: cmdName,
                valuesArray:  args
            } 
        })

        if(resolve != undefined && reject != undefined){
            queueSend.push({cmdName:  cmdName, timeout: setTimeout(reject,ms), resolve: resolve, reject: reject});
            waitingResponse = true;
        }
    },
    setActuator(id, value,resolve, reject)
    {
        let state = 'cansat.resources.close'
        if(value== 1){
            state = 'cansat.resources.open'
        }
        if(id == 0){
            this.sendCMDToWorker('setParachute', [value],resolve,reject)

            store.commit('setActuators',
            { 
                'cansatIndex': 0, 
                'actuatorIndex': 0, 
                'status' : state
            })
        }else{
            this.sendCMDToWorker('setBalloon', [value],resolve,reject)
            store.commit('setActuators',
            { 
                'cansatIndex': 0, 
                'actuatorIndex': id, 
                'status' : state
            })
        }
    },
    startMission(){
        worker.send( { 
            cmdToWorker:{
                cmd: 'startReport'
            } 
        })
    },
    stopMission(){
        worker.send( { 
            cmdToWorker:{
                cmd: 'endReport'
            } 
        })
    },

    connectToPort(comName){
        worker.send( { 
            cmdToWorker:{
                cmd: 'connectSerialPort',
                comName: comName
            } 
        })
    },

    disconnectToPort(){
        worker.send( { 
            cmdToWorker:{
                cmd: 'disconnectSerialPort'
            } 
        })
    },

    connectToCansat(resolve, reject){
        this.sendCMDToWorker('getBattery', [], resolve, reject)
    },

    disconnectToCansat(){
        worker.send( { 
            cmdToWorker:{
                cmd: 'disconnectToCansat'
            } 
        })
    },

    getSensors(data,resolve, reject){
        this.sendCMDToWorker('getSensor', data, resolve, reject)
    },

    enablePowerSupply(resolve,reject){
        this.sendCMDToWorker('enablePowerSupply', [1], resolve, reject)
    },
    newCMDReceived(msg){
        if(msg.newCMDReceived &&  msg.newCMDReceived.data){
            let cmdsToParse = msg.newCMDReceived.data.decoded
            console.log("Decoding new packet");
            for(let c= 0; c < cmdsToParse.length; c++){   
                if(queueSend.length > 0){
                    queueRec.push({cmdName: cmdsToParse[c].cmdName});
                    let index = -1;
                    for(let r = 0; r< queueRec.length; r++){
                        if(queueRec[r].cmdName == queueSend[0].cmdName){
                            let send = queueSend.shift()
                            clearTimeout(send.timeout)
                            if(send.resolve)send.resolve()
                            index = r;
                            break
                        }
                    }
                    if(index >= 0){
                        queueRec = queueRec.slice(index)
                    }

                }else{
                    //debugger
                }

                switch(cmdsToParse[c].cmdName){
                    case 'getError':
                        let code = cmdsToParse[c].error.code;
                        let contentError = vm.$t(cmdsToParse[c].error.content);

                        if(cmdsToParse[c].error.info)
                        {
                            contentError += ' ' +  vm.$t(cmdsToParse[c].error.info) ;
                        }
                        
                        store.commit('pushNotificationModal',{ 
                            'title': vm.$t(cmdsToParse[c].error.title), 
                            'content': contentError,
                            'date': utils.getDate(),
                            'code': code,
                            'okText': vm.$t('cansat.notifications.modal.okBtn'),
                            'uuid': utils.generateUUID().toString(),
                            'type': vm.$t('cansat.notifications.center.types.error'),
                            'cancelDisabled': true
                        })

                        
                        if( code != 1 && code!= 2 && code != 3 ) // If exist and error on the power supplies
                        {
                            let filtered = defaultSensors.getSensors().filter(function(n) {
                                return (n._type == 'power')
                            })
                            for(let s = 0 ;s < filtered.length;s++){
                                filtered[s].status= 'danger'
                                store.commit('setSensor', filtered[s])
                            }
                        }

                        this.sendCMDToWorker('getError', [], null,null)

                        break;
                    case 'getParachute':
                        if(cmdsToParse[c].state){ // Parachute is open
                            store.commit('setActuators',
                            { 
                                'cansatIndex': 0, 
                                'actuatorIndex': 0, 
                                'status' : 'cansat.resources.open'
                            })
                            store.commit('pushNotificationToast',{ 
                                'text': vm.$t('cansat.worker.actuators.openParachute'), 
                                'icon': 'fa-check'          
                            })
                            /*
                            store.commit('pushNotificationModal',{ 
                                'title': vm.$t('cansat.worker.actuators.openParachute'), 
                                'date': utils.getDate(),
                                'code': 0,
                                'uuid': utils.generateUUID().toString(),
                                'type': vm.$t('cansat.notifications.center.types.info')
                            })*/
                        }else{                     // It's close
                            store.commit('setActuators',
                            { 
                                'cansatIndex': 0, 
                                'actuatorIndex': 0, 
                                'status' : 'cansat.resources.close'
                            })
                            store.commit('pushNotificationToast',{ 
                                'text': vm.$t('cansat.worker.actuators.closeParachute'), 
                                'icon': 'fa-check'          
                            })
                            /*
                            store.commit('pushNotificationModal',{ 
                                'title': vm.$t('cansat.worker.actuators.closeParachute'), 
                                'date': utils.getDate(),
                                'code': 0,
                                'uuid': utils.generateUUID().toString(),
                                'type': vm.$t('cansat.notifications.center.types.info')
                            })*/
                        }
                        break;
                    case 'setParachute':
         
                        break; 
                    case 'getBalloon':
                        if(cmdsToParse[c].state){ // Balloon is open
                            $store.commit('setActuators',
                            { 
                                'cansatIndex': 0, 
                                'actuatorIndex': 1, 
                                'status' : 'cansat.resources.open'
                            })
                            store.commit('pushNotificationToast',{ 
                                'text': vm.$t('cansat.worker.actuators.openBalloon'), 
                                'icon': 'fa-check'          
                            })
                            /*
                            store.commit('pushNotificationModal',{ 
                                'title': vm.$t('cansat.worker.actuators.openBalloon'), 
                                'date': utils.getDate(),
                                'code': 0,
                                'uuid': utils.generateUUID().toString(),
                                'type': vm.$t('cansat.notifications.center.types.info')
                            })
                            */
                        }else{                     // It's close
                            store.commit('setActuators',
                            { 
                                'cansatIndex': 0, 
                                'actuatorIndex': 1, 
                                'status' : 'cansat.resources.close'
                            })
                            store.commit('pushNotificationToast',{ 
                                'text': vm.$t('cansat.worker.actuators.closeBalloon'), 
                                'icon': 'fa-check'          
                            })
                            /*
                            store.commit('pushNotificationModal',{ 
                                'title': vm.$t('cansat.worker.actuators.closeBalloon'), 
                                'date': utils.getDate(),
                                'code': 0,
                                'uuid': utils.generateUUID().toString(),
                                'type': vm.$t('cansat.notifications.center.types.info')
                            })*/
                        }
                        break;
                    case 'setBalloon':

                        break;  
                    case 'getSensor':
                        if( cmdsToParse[c].lat != undefined){ // GPS sensor
                            store.commit('addNewLocation',{ 
                                timespan: utils.getDate(),
                                lat: cmdsToParse[c].lat,
                                lng: cmdsToParse[c].lng 
                            })
                        }else{
                            let sensor;
                            let index = -1
                            let searchID
                            if(store.getters.axtec.project.cansat[0].missionActive ){
                                searchID = store.getters.axtec.project.mission.data.sensors
                            }else{
                                searchID = store.getters.axtec.project.cansat[0].sensors
                            }
                            for(let s = 0; s <searchID.length; s++ ){
                                if(searchID[s].id == cmdsToParse[c].sensorID){
                                    index = s
                                    sensor = searchID[s];
                                    break;
                                }
                            }
                            if(sensor != undefined){                                  
                                if(index != -1){
                                    if(sensor._type == 'vector'){
                                        store.commit('setSensor',{
                                            cansatIndex: 0 ,
                                            sensorIndex: index,
                                            x: cmdsToParse[c].x,
                                            y: cmdsToParse[c].y,
                                            z: cmdsToParse[c].z
                                        })
                                        
                                        store.commit('addSensorSample',{
                                            index: index,
                                            samples: {
                                                x: cmdsToParse[c].x,
                                                y: cmdsToParse[c].y,
                                                z: cmdsToParse[c].z,
                                                timespan: utils.getDate()
                                            }
                                        })
                                        
                                    }else{
                                        store.commit('setSensor', {
                                            cansatIndex: 0 ,
                                            sensorIndex: index,
                                            lastValue: cmdsToParse[c].value
                                        })
                                        
                                        store.commit('addSensorSample',{
                                            index: index,
                                            samples: {
                                                lastValue: cmdsToParse[c].value,
                                                timespan: utils.getDate()
                                            }
                                        })
                                    }
                                }                                
                            }else{
                                debugger
                            }
                        }
                        break;
                    case 'getBattery':
                        store.commit('setBatteryLevel', { level: cmdsToParse[c].state })
                        break; 
                    case 'setReport':
                        break;
                    case 'enablePowerSupply':
                        break;                                  
                }
            }
        }
    }
}
