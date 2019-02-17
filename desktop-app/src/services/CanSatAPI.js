const { fork } = require('child_process');

const isDevelopment = process.env.NODE_ENV !== 'production'

import store from '../store'
import utils from 'services/utils'
import { Debugger } from 'electron';

let worker = undefined

export default {
    initWorker()
    {
        worker =  fork(require.resolve('./worker.js'));

        worker.on('message', (msg) => {
            console.log(JSON.stringify(msg))
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
            worker = undefined
            let that = this
            setTimeout(that.initWorker, 5000);
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
                    'icon': 'fa-check'          
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
                debugger
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