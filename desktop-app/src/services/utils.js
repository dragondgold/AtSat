const log = require('electron-log')
const fs = require('fs')
const mkdirp = require('mkdirp')
const getDirName = require('path').dirname

export default {
  hex2rgb (hex, opacity) {
    hex = (hex + '').trim()

    let rgb = null
    let match = hex.match(/^#?(([0-9a-zA-Z]{3}){1,3})$/)

    if (!match) {
      return null
    }

    rgb = {}

    hex = match[1]

    if (hex.length === 6) {
      rgb.r = parseInt(hex.substring(0, 2), 16)
      rgb.g = parseInt(hex.substring(2, 4), 16)
      rgb.b = parseInt(hex.substring(4, 6), 16)
    } else if (hex.length === 3) {
      rgb.r = parseInt(hex.substring(0, 1) + hex.substring(0, 1), 16)
      rgb.g = parseInt(hex.substring(1, 2) + hex.substring(1, 2), 16)
      rgb.b = parseInt(hex.substring(2, 3) + hex.substring(2, 3), 16)
    }

    rgb.css = 'rgb' + (opacity ? 'a' : '') + '('
    rgb.css += rgb.r + ',' + rgb.g + ',' + rgb.b
    rgb.css += (opacity ? ',' + opacity : '') + ')'

    return rgb
  },
  createFile(path, contents){
    mkdirp(getDirName(path), (err) => {
      if (err){
        alert("An error ocurred creating the folder "+ err.message)
      }else{
        fs.writeFile(path, contents, (err) => {
          if(err){
            log.error('An error ocurred creating the file ' + path + ' error: ' +  err.message);
            alert("An error ocurred creating the file "+ err.message)
          }else{
            log.info('The file ' + path + 'has been succesfully saved');
            alert("The file has been succesfully saved")
          }  
        }) 
      }
    })
  },
  loadFile(path){
    if (fs.existsSync(path)) {
      fs.readFile(path,'utf8', function read(err, data) {
        if (err) {
          log.error('An error ocurred reading the file ' + path + ' error: ' +  err.message);
          alert("An error ocurred reading the file "+ err.message)
          return ''
        }else{
          console.log(data)
          return data
        }
      }) 
    }else{
      return '' // File doesn't exist
    }
  }
}
