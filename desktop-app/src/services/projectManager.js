const log = require('electron-log')
const fs = require('fs')
const mkdirp = require('mkdirp')
const getDirName = require('path').dirname
const {dialog} = require('electron').remote

import utils from '../services/utils'
import store from '../store'

export default {
    openProject(){  
        if(store.getters.axtec.project.path !== ''){
            this.showSaveProject()
        }else{
            this.openDialog()
        }
    },
    showSaveProject(){
        store.commit('pushNotificationModal',{ 
            'title': 'Desea crear un nuevo proyecto', 
            'date': utils.getDate(),
            'content': '¿Esta seguro que desea crear un nuevo proyecto?. Desea guardar los cambios y continuar',
            'code': 0,
            'okCallback': this.openDialog,
            'okText': 'Continuar',
            'cancelText': 'Cancelar',
            'uuid':  utils.generateUUID(),
            'type': 'info',
        })
    },
    openDialog() {

        let path =
        dialog.showOpenDialog({ 
            properties: [ 
                'openFile',
            ],        
            filters: [
            {name: 'Custom File Type', extensions: ['cansat_pro']},
            ]
        })
        if(path != null){
            if(store.getters.axtec.project.path == ''){
                let newProject = this.loadFile(path[0])
            }else{
            
            }     
        }
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
                    if(store.getters.axtec.consoleDebug){
                        console.log(data)
                    }         
                    return data
                }
            }) 
        }else{
            return '' // File doesn't exist
        }
    }
}
