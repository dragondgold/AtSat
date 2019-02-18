const { fork } = require('child_process');

const isDevelopment = process.env.NODE_ENV !== 'production'

import store from '../store'
import utils from 'services/utils'
import defaultSensors from 'data/Sensors.js'

let worker = undefined

let defaultCmds = undefined

export default {
    initWorker()
    {
        
        worker =  fork(require.resolve('./worker.js'));

        worker.on('message', (msg) => {
            console.log(JSON.stringify(msg))
            this.etConnected(msg);
            this.cansatConnected(msg);
            this.newCMDReceived(msg);
        });

        worker.on('error', (err) => {
            debugger
            console.log("\n\t\tERROR: spawn failed! (" + err + ")")
        });

        worker.on('exit', (e) => {
            debugger
            console.log("Exit fork: " + e)
            worker = undefined
            let that = this
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
    getDefaultSensors()
    {
        worker.send('getDefaultSensors');
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
                    'text': vm.$t('cansat.notifications.etConnected'), 
                    'icon': 'fa-check'          
                })
                store.commit('setStatusCanSat', true)
            }else if(!msg.cansat.connected  && msg.cansat.connected != store.getters.axtec.project.cansat[0].connected )
            {
                store.commit('pushNotificationToast',{ 
                    'text': vm.$t('cansat.notifications.etDisconnected'), 
                    'icon': 'fa-unlink'          
                })
                store.commit('setStatusCanSat', false)
            }
        }
    },
    sendCMDToWorker(cmdName, args){
        worker.send( { 
            cmdToSend:{
                cmd: cmdName,
                valuesArray:  args
            } 
        })
    },
    setActuator(id, value)
    {
        if(id == 0){
            this.sendCMDToWorker('setParachute', [value])
        }else{
            this.sendCMDToWorker('setBalloon', [value])
        }
    },
    startMission(){
        this.sendCMDToWorker('startReport', [1])
    },
    stopMission(){
        this.sendCMDToWorker('startReport', [0])
    },
    newCMDReceived(msg){
        if(msg.newCMDReceived){
            let cmdsToParse = msg.newCMDReceived.data.decoded
            for(let c= 0; c < cmdsToParse.length; c++){
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
                            store.commit('pushNotificationModal',{ 
                                'title': vm.$t('cansat.worker.actuators.openParachute'), 
                                'date': utils.getDate(),
                                'code': 0,
                                'uuid': utils.generateUUID().toString(),
                                'type': vm.$t('cansat.notifications.center.types.info')
                            })
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
                            store.commit('pushNotificationModal',{ 
                                'title': vm.$t('cansat.worker.actuators.closeParachute'), 
                                'date': utils.getDate(),
                                'code': 0,
                                'uuid': utils.generateUUID().toString(),
                                'type': vm.$t('cansat.notifications.center.types.info')
                            })
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
                            store.commit('pushNotificationModal',{ 
                                'title': vm.$t('cansat.worker.actuators.openBalloon'), 
                                'date': utils.getDate(),
                                'code': 0,
                                'uuid': utils.generateUUID().toString(),
                                'type': vm.$t('cansat.notifications.center.types.info')
                            })
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
                            store.commit('pushNotificationModal',{ 
                                'title': vm.$t('cansat.worker.actuators.closeBalloon'), 
                                'date': utils.getDate(),
                                'code': 0,
                                'uuid': utils.generateUUID().toString(),
                                'type': vm.$t('cansat.notifications.center.types.info')
                            })
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
                            let sensor = defaultSensors.getSensors().filter(function(n) {
                                return (n.id == cmdsToParse[c].sensorID)
                            })
                            if(sensor[0]){   
                                if(sensor[0]._type == 'vector'){

                                    sensor[0].x = cmdsToParse[c].x
                                    sensor[0].y = cmdsToParse[c].y
                                    sensor[0].z = cmdsToParse[c].z

                                    store.commit('setSensor', sensor[0])
                                    
                                    store.commit('addSensorSample',{
                                        index: cmdsToParse[c].sensorID,
                                        samples: {
                                            x: cmdsToParse[c].x,
                                            y: cmdsToParse[c].y,
                                            z: cmdsToParse[c].z,
                                            timespan: utils.getDate()
                                        }
                                    })
                                    
                                }else{
                                    
                                    sensor[0].lastValue = cmdsToParse[c].value

                                    store.commit('setSensor', sensor[0])
                                    
                                    store.commit('addSensorSample',{
                                        index: cmdsToParse[c].sensorID,
                                        samples: {
                                            lastValue: cmdsToParse[c].value,
                                            timespan: utils.getDate()
                                        }
                                    })
                                }
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
