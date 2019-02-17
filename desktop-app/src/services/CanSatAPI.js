const ref = require("ref")
const path = require('path')
const { fork } = require('child_process');

const isDevelopment = process.env.NODE_ENV !== 'production'

import store from '../store'
import utils from 'services/utils'

let worker = undefined

export default {
    initWorker()
    {
        worker =  fork(require.resolve('./worker.js'));

        worker.on('message', (msg) => {
            this.etConnected(msg);
            this.cansatConnected(msg);
            this.anErrorOcurred(msg);
            this.newSample(msg);
        });

        worker.on('error', (err) => {
            debugger
            console.log("\n\t\tERROR: spawn failed! (" + err + ")")
        });

        worker.on('exit', (e) => {
            debugger
            console.log("Exit fork: " + e)
        });

    },
    killWorker()
    {
        if(worker != undefined){
            worker.kill()
        }
    },
    sendCommand(cmd, data)
    {
        worker.send({ 
            sendToCanSat: { 
                cmd: cmd,
                data: data
            }
        })
    },
    etConnected(msg)
    {
        if(msg.et)
        {
            if(msg.et.connected  && msg.et.connected != store.getters.axtec.project.earthStation.connected ){
                store.commit('pushNotificationToast',{ 
                    'text': vm.$t('cansat.notifications.etConnected'), 
                    'icon': 'fa-check'          
                })
                store.commit('pushNotificationModal',{ 
                    'title': vm.$t('cansat.notifications.etConnected'), 
                    'date': utils.getDate(),
                    'code': 0,
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.info')
                })
                store.commit('setStatusEarthStation', true)
            }else if(!msg.et.connected  && msg.et.connected != store.getters.axtec.project.earthStation.connected){
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
    anErrorOcurred(msg)
    {
        if(msg.error)
        {

        }
    },
    newSample(msg)
    {
        if(msg.sample != undefined && msg.error!= undefined && msg.error == 0)
        {
            store.commit('addSensorSample',{
                index: msg.data.sensorID -1,
                samples: {
                    x: msg.data.x,
                    y: msg.data.y,
                    z: msg.data.z,
                    timespan: utils.getDate()
                }
            })
        }
    }

}