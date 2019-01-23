const log = require('electron-log')
const fs = require('fs')
const mkdirp = require('mkdirp')
const getDirName = require('path').dirname
const {dialog} = require('electron').remote

import utils from '../services/utils'
import store from '../store'
import Vue from 'vue'

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
            'title': vm.$t('cansat.notifications.modal.project.titleNew'), 
            'date': utils.getDate(),
            'content': vm.$t('cansat.notifications.modal.project.contentNew'),
            'code': 0,
            'okCallback': vm.openDialog,
            'okText': vm.$t('cansat.notifications.modal.okBtn'),
            'cancelText': vm.$t('cansat.notifications.modal.cancelBtn'),
            'uuid': utils.generateUUID().toString(),
            'type': vm.$t('cansat.notifications.center.types.info')
        })
    },
    openDialog() {

        let path =
        dialog.showOpenDialog({ 
            'title': vm.$t('cansat.dialog.open'),
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
                    if(store.getters.axtec.debug){
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
