const ref = require("ref")
const ffi = require("ffi")
const path = require('path')

const MCP2210 = ffi.Library(path.resolve('./src/MCP2210/mcp2210_dll_um_x64.dll'), {
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
        console.log('MCP2210 Last error code: ' + code)
        return code
    },
    getConnectedDevCount () {
        let vid = 0x4D8
        let pid = 0xDE
        let devCount = MCP2210.Mcp2210_GetConnectedDevCount(vid,pid)
        console.log('MCP2210 Connected devs: ' + devCount)
        return devCount
    }
}