const path = require('path')

const { fork } = require('child_process');

const isDevelopment = process.env.NODE_ENV !== 'production'

let pathMCP2210CLI = path.resolve('./build/MCP2210/MCP2210CLI.exe')
if(!isDevelopment){
    pathMCP2210CLI = path.resolve('./resources/build/MCP2210/MCP2210CLI.exe')
}

let worker = undefined

import store from '../store'
import utils from 'services/utils'

export default {
    initWorker(){
        worker =  fork(require.resolve('./worker.js'));

        worker.on('message', (msg) => {
            if(msg.cmd != undefined && msg.error!= undefined && msg.error == 0){
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
        });

        worker.on('error', (err) => {
            console.log("\n\t\tERROR: spawn failed! (" + err + ")")
        });

        worker.on('exit', (e) => {
            debugger
            console.log("Exit fork: " + e)
        });
        worker.send("First Fun");

    },
    killWorker(){
        if(worker != undefined){
            worker.kill()
        }
    },
    sendCommand(cmd, data){
        worker.send({ 
            sendToCanSat: { 
                cmd: cmd,
                data: data
            }
        })
    }
}