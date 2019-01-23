const ref = require("ref")
const ffi = require("ffi")
const path = require('path')

import store from '../store'

const isDevelopment = process.env.NODE_ENV !== 'production'

let pathMCP2210 = path.resolve('./build/MCP2210/mcp2210_dll_um_x64.dll')
if(!isDevelopment){
    pathMCP2210 = path.resolve('./resources/build/MCP2210/mcp2210_dll_um_x64.dll')
}

const MCP2210 = ffi.Library(pathMCP2210, {
  "Mcp2210_GetLibraryVersion": ['int',['pointer']],  
  "Mcp2210_GetLastError": ['int', ['void']], 
  "Mcp2210_GetConnectedDevCount": ['int', ['uint', 'uint']]
})

export default {
    getLibraryVersion (pointer) {
        let v = MCP2210.Mcp2210_GetLibraryVersion(pointer)
        return pointer
    },
    getLastError () {
        let code = MCP2210.Mcp2210_GetLastError(0)
        if(store.getters.axtec.debug){
            console.log('MCP2210 Last error code: ' + code)
        }
        return code
    },
    getConnectedDevCount () {
        let vid = 0x4D8
        let pid = 0xDE
        let devCount = MCP2210.Mcp2210_GetConnectedDevCount(vid,pid)
        if(store.getters.axtec.debug){
            console.log('MCP2210 Connected devs: ' + devCount)
        } 
        return devCount
    }
}