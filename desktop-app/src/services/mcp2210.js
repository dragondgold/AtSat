import store from '../store'

const { fork } = require('child_process');

const exec = require('child_process').execFile
const path = require('path')
const isDevelopment = process.env.NODE_ENV !== 'production'


let pathMCP2210 = path.resolve('./build/MCP2210/mcp2210_dll_um_x64.dll')
let pathMCP2210CLI = path.resolve('./build/MCP2210/MCP2210CLI.exe')
if(!isDevelopment){
    pathMCP2210 = path.resolve('./resources/build/MCP2210/mcp2210_dll_um_x64.dll')
    pathMCP2210CLI = path.resolve('./resources/build/MCP2210/MCP2210CLI.exe')
}

const vid = 0x4D8
const pid = 0xDE
const ok = 0
const error = -1

let forked = undefined

export default {
    connectByIndex (index) {
        exec(pathMCP2210CLI , ['-ConnectI=' + index], function(err, data) {  
                              
        }); 
    },
    setGPIO(pin,state){ // pin = '0' to '8' and state = 'high' or 'low'

        exec(pathMCP2210CLI , ['-gpioW=gp' + pin + state], function(err, data) {  
                           
        }); 
    },
    spiTransfer(){
        exec(pathMCP2210CLI , [ '-spitxfer=40,0A,55,00 -bd=6000000 -cs=gp0 -idle=ffff -actv=ffef -f="dataToReceive.txt"'], function(err, data) {  
                           
        }); 
        
    },
    test(){
 
        forked =  fork(require.resolve('./worker.js'));

        forked.on('message', (msg) => {
          console.log('Message from child', msg);
        });

        forked.on('error', (err) => {
            console.log("\n\t\tERROR: spawn failed! (" + err + ")");
          });

          forked.on('exit', (e) => {
            console.log("Exit fork: " + e);
          });

        forked.send({ hello: 'world' });
    },
    closeWorker(){
        if(forked != undefined){
            forked.kill()
        }
    }
}