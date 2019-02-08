const log = require('electron-log')
const fs = require('fs')
const mkdirp = require('mkdirp')
const getDirName = require('path').dirname
const {dialog} = require('electron').remote

import utils from '../services/utils'
import store from '../store'
import Vue from 'vue'
import defaultSensors from 'data/Sensors'
import codeValues from 'data/codeValues'

export default {
    saveMission(){
        if(store.getters.axtec.project.path !== ''){
            let jsonToEdit = JSON.parse(JSON.stringify(store.getters.axtec.project))
            let sensors = jsonToEdit.cansat[0].sensors
            for(let s= 0; s< sensors.length; s++){
                if(sensors[s]._type == 'vector'){
                    sensors[s].x = 0
                    sensors[s].y = 0
                    sensors[s].z = 0
                }else{ // It's 'scalar' or 'power' or 'user'
                    sensors[s].lastValue = sensors[s].minValue
                }
                delete sensors[s].samples
            }
            let projectFile =  JSON.stringify({ sensors: sensors})
            this.saveProjectFile(store.getters.axtec.project.path,projectFile)
        }else{
            store.commit('pushNotificationModal',{ 
                'title': vm.$t('cansat.notifications.modal.project.save'), 
                'date': utils.getDate(),
                'code': codeValues.project.invalidPath,
                'okText': vm.$t('cansat.notifications.modal.okBtn'),
                'uuid': utils.generateUUID().toString(),
                'type': vm.$t('cansat.notifications.center.types.error'),
                'cancelDisabled': true
            })
        }
    },
    saveProject(){  
        if(store.getters.axtec.project.path !== ''){
            let jsonToEdit = JSON.parse(JSON.stringify(store.getters.axtec.project))
            let sensors = jsonToEdit.cansat[0].sensors
            for(let s= 0; s< sensors.length; s++){
                if(sensors[s]._type == 'vector'){
                    sensors[s].x = 0
                    sensors[s].y = 0
                    sensors[s].z = 0
                }else{ // It's 'scalar' or 'power' or 'user'
                    sensors[s].lastValue = sensors[s].minValue
                }
                delete sensors[s].samples
            }
            let projectFile =  JSON.stringify({ sensors: sensors})
            this.saveProjectFile(store.getters.axtec.project.path,projectFile)
        }else{
            store.commit('pushNotificationModal',{ 
                'title': vm.$t('cansat.notifications.modal.project.save'), 
                'content': vm.$t('cansat.notifications.modal.project.noProjectCreated'), 
                'date': utils.getDate(),
                'code': codeValues.project.saveError,
                'okText': vm.$t('cansat.notifications.modal.okBtn'),
                'uuid': utils.generateUUID().toString(),
                'type': vm.$t('cansat.notifications.center.types.error'),
                'cancelDisabled': true
            })
        }
    },

    openProjectDialog(overwrite) {
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
            if(store.getters.axtec.project.path == '' || overwrite){
                this.loadFile(path[0],this.validateProject)
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
                        log.info('The file ' + path + ' has been succesfully saved');
                        alert('The file ' + path + ' has been succesfully saved')
                    }  
                }) 
            }
        })
    },
    saveProjectFile(path, contents){
        mkdirp(getDirName(path), (err) => {
            if (err){
                alert("An error ocurred creating the folder "+ err.message)
            }else{
                fs.writeFile(path, contents, (err) => {
                if(err){
                    log.error('An error ocurred creating the file ' + path + ' error: ' +  err.message);
                    store.commit('pushNotificationModal',{ 
                        'title': vm.$t('cansat.notifications.modal.project.saveError'), 
                        'content': '',
                        'date': utils.getDate(),
                        'code': 0,
                        'okText': vm.$t('cansat.notifications.modal.okBtn'),
                        'uuid': utils.generateUUID().toString(),
                        'type': vm.$t('cansat.notifications.center.types.error'),
                        'cancelDisabled': true
                    })
                }else{
                    log.info('The file ' + path + 'has been succesfully saved');
                    store.commit('pushNotificationModal',{ 
                        'title': vm.$t('cansat.notifications.modal.project.saveOk'), 
                        'date': utils.getDate(),
                        'code': codeValues.project.ok,
                        'uuid': utils.generateUUID().toString(),
                        'type': vm.$t('cansat.notifications.center.types.info')
                    })
                    store.commit('pushNotificationToast',{ 
                        'text': vm.$t('cansat.notifications.modal.project.saveOk'), 
                        'icon': 'fa-check'          
                    })
                }  
                }) 
            }
        })
    },
    loadFile(path, callback){   
        if (fs.existsSync(path)) {
            fs.readFile(path,'utf8', function read(err, data) {
                if (err) {
                    log.error('An error ocurred reading the file ' + path + ' error: ' +  err.message);
                    alert("An error ocurred reading the file "+ err.message)
                    return ''
                }else{
                    if(store.getters.axtec.debug){
                        console.log(data,path)
                    }
                    if(callback){
                        let code = callback(data,path)
                        console.log('LOAD PROJECT CODE: ' + code)
                    }         
                    
                }
            }) 
        }else{
            return '' // File doesn't exist
        }
    },
    canCreateFile(path){
        if (fs.existsSync(path)) {
            return false
        }
        return true
    },
    validateProject(data, path){
        try {
            JSON.parse(data)
        } catch (e) {
            store.commit('pushNotificationModal',{ 
                'title': vm.$t('cansat.notifications.modal.project.open'), 
                'content': vm.$t('cansat.notifications.modal.project.invalidFormat'),
                'date': utils.getDate(),
                'code': codeValues.project.jsonInvalid,
                'okText': vm.$t('cansat.notifications.modal.okBtn'),
                'uuid': utils.generateUUID().toString(),
                'type': vm.$t('cansat.notifications.center.types.error'),
                'cancelDisabled': true
            })
            return codeValues.project.jsonInvalid
        }
        let project = JSON.parse(data)

        // Invalid project 
        if(project.sensors == undefined || project.sensors.length == 0){ 
            store.commit('pushNotificationModal',{ 
                'title': vm.$t('cansat.notifications.modal.project.open'), 
                'content': vm.$t('cansat.notifications.modal.project.invalidContent'),
                'date': utils.getDate(),
                'code': codeValues.project.projectKeyInvalid,
                'okText': vm.$t('cansat.notifications.modal.okBtn'),
                'uuid': utils.generateUUID().toString(),
                'type': vm.$t('cansat.notifications.center.types.error'),
                'cancelDisabled': true
            })
            return codeValues.project.projectKeyInvalid
        }else{
            let sensors = project.sensors
            
            // Incomplete sensors with all _types
            let missingCommonKeys = sensors.filter(function(s,index) { 
                return (s._type == undefined || s.id == undefined || s.status == undefined || s.cansatIndex == undefined || 
                        s.step == undefined || s.unit == undefined || s.type == undefined || s.maxValue == undefined ||
                        s.minValue == undefined || s.minThreshold == undefined || s.maxThreshold == undefined 
                )
            })
            if(missingCommonKeys.length > 0){ 

                store.commit('pushNotificationModal',{ 
                    'title': vm.$t('cansat.notifications.modal.project.open'), 
                    'content': vm.$t('cansat.notifications.modal.project.incompleteFormat'),
                    'date': utils.getDate(),
                    'code': codeValues.project.missingCommonKeys,
                    'okText': vm.$t('cansat.notifications.modal.okBtn'),
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.error'),
                    'cancelDisabled': true
                })
                return codeValues.project.missingCommonKeys
            }

            // Missing default sensors
            let defaultVectorSensors = defaultSensors.sensors.filter(function(s,index) {   
                return (s._type == 'vector')
            })
            let defaultScalarSensors = defaultSensors.sensors.filter(function(s,index) {   
                return (s._type == 'scalar')
            })
            let defaultPowerSensors = defaultSensors.sensors.filter(function(s,index) {    
                return (s._type == 'power')
            })
            let vectorSensors = sensors.filter(function(s,index) {   
                return (s._type == 'vector')
            })
            let scalarSensors = sensors.filter(function(s,index) {    
                return (s._type == 'scalar')
            })
            let powerSensors = sensors.filter(function(s,index) {  
                return (s._type == 'power')
            })
            if(defaultVectorSensors.length != vectorSensors.length || defaultScalarSensors.length != scalarSensors.length || defaultPowerSensors.length != powerSensors.length){
                store.commit('pushNotificationModal',{ 
                    'title': vm.$t('cansat.notifications.modal.project.open'), 
                    'content': vm.$t('cansat.notifications.modal.project.missingSensors'),
                    'date': utils.getDate(),
                    'code': codeValues.project.missingSensors,
                    'okText': vm.$t('cansat.notifications.modal.okBtn'),
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.error'),
                    'cancelDisabled': true
                })
                return codeValues.project.missingSensors
            }




            // Find duplicates ids
            let s = sensors.map(function(item){ return item.id });
            let isDuplicate = s.some(function(item, idx){ 
                return s.indexOf(item) != idx 
            });
            if(isDuplicate){
                store.commit('pushNotificationModal',{ 
                    'title': vm.$t('cansat.notifications.modal.project.open'), 
                    'content': vm.$t('cansat.notifications.modal.project.duplicateIds'),
                    'date': utils.getDate(),
                    'code': codeValues.project.duplicateIDs,
                    'okText': vm.$t('cansat.notifications.modal.okBtn'),
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.error'),
                    'cancelDisabled': true
                })
                return codeValues.project.duplicateIDs
            }

            // Find wrong IDs
            for(let s= 0 ;s < defaultSensors.sensors.length;s++){
                let matchId = sensors.filter(function(sensor,index) { 
                    return (sensor.id == defaultSensors.sensors[s].id && sensor._type == defaultSensors.sensors[s]._type) 
                })
                if(matchId.length !=1){
                    store.commit('pushNotificationModal',{ 
                        'title': vm.$t('cansat.notifications.modal.project.open'), 
                        'content': vm.$t('cansat.notifications.modal.project.wrongID'),
                        'date': utils.getDate(),
                        'code': codeValues.project.wrongIDs,
                        'okText': vm.$t('cansat.notifications.modal.okBtn'),
                        'uuid': utils.generateUUID().toString(),
                        'type': vm.$t('cansat.notifications.center.types.error'),
                        'cancelDisabled': true
                    })
                    return codeValues.project.wrongIDs
                }
            }

            // It's a valid project
            store.commit('pushNotificationModal',{ 
                'title': vm.$t('cansat.notifications.modal.project.loadedOk'), 
                'date': utils.getDate(),
                'code': 0,
                'uuid': utils.generateUUID().toString(),
                'type': vm.$t('cansat.notifications.center.types.info')
            })
            store.commit('pushNotificationToast',{ 
                'text': vm.$t('cansat.notifications.modal.project.loadedOk'), 
                'icon': 'fa-check'          
            })

            sensors[0].clear = true // To clear sensors list

            for(let s= 0; s < sensors.length;s++){
                store.commit('addNewSensor', sensors[s])
            }
            
            store.commit('axtecPath', path)

            store.commit('setTestStatus',{ 
                cansatIndex: 0, 
                testOk: false
            })

            vm.$router.push({ name: 'linkSat' })
            return codeValues.project.ok
        }
    }
}
