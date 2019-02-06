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
        if(store.getters.axtec.project.mission.path != ''){
            
            let mission = JSON.parse(JSON.stringify(store.getters.axtec.project.mission))

            delete mission.path

            let missionFile =  JSON.stringify({ 
                mission: mission
            })
            this.saveMissionFile(store.getters.axtec.project.mission.path,missionFile)
        }else{
            store.commit('pushNotificationModal',{ 
                'title': vm.$t('cansat.notifications.modal.mission.save'), 
                'content': vm.$t('cansat.notifications.modal.mission.noMissionCreated'), 
                'date': utils.getDate(),
                'code': codeValues.mission.saveError,
                'okText': vm.$t('cansat.notifications.modal.okBtn'),
                'uuid': utils.generateUUID().toString(),
                'type': vm.$t('cansat.notifications.center.types.error'),
                'cancelDisabled': true
            })
        }
    },
    saveMissionFile(path, contents){
        mkdirp(getDirName(path), (err) => {
            if (err){
                alert("An error ocurred creating the folder "+ err.message)
            }else{
                fs.writeFile(path, contents, (err) => {
                if(err){
                    log.error('An error ocurred creating the file ' + path + ' error: ' +  err.message);
                    store.commit('pushNotificationModal',{ 
                        'title': vm.$t('cansat.notifications.modal.mission.saveError'), 
                        'content': '',
                        'date': utils.getDate(),
                        'code': codeValues.mission.saveError,
                        'okText': vm.$t('cansat.notifications.modal.okBtn'),
                        'uuid': utils.generateUUID().toString(),
                        'type': vm.$t('cansat.notifications.center.types.error'),
                        'cancelDisabled': true
                    })
                }else{
                    log.info('The file ' + path + 'has been succesfully saved');
                    store.commit('pushNotificationModal',{ 
                        'title': vm.$t('cansat.notifications.modal.mission.saveOk'), 
                        'date': utils.getDate(),
                        'code': codeValues.mission.ok,
                        'uuid': utils.generateUUID().toString(),
                        'type': vm.$t('cansat.notifications.center.types.info')
                    })
                    store.commit('pushNotificationToast',{ 
                        'text': vm.$t('cansat.notifications.modal.mission.saveOk'), 
                        'icon': 'fa-check'          
                    })
                }  
                }) 
            }
        })
    },
    openMissionDialog(overwrite) {
        let path =
        dialog.showOpenDialog({ 
            'title': vm.$t('cansat.dialog.openMission'),
            properties: [ 
                'openFile',
            ],        
            filters: [
            {name: 'Custom File Type', extensions: ['cansat_data']},
            ]
        })
        if(path != null){
            if(store.getters.axtec.project.mission.path == '' || overwrite){
                this.loadMissionFile(path[0],this.validateMission)
            }else{
            
            }     
        }
    }, 
    loadMissionFile(path, callback){   
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
                        console.log('LOAD MISSION CODE: ' + code)
                    }         
                    
                }
            }) 
        }else{
            return '' // File doesn't exist
        }
    }, 
    validateMission(data, path){
        try {
            JSON.parse(data)
        } catch (e) {
            store.commit('pushNotificationModal',{ 
                'title': vm.$t('cansat.notifications.modal.mission.import'), 
                'content': vm.$t('cansat.notifications.modal.mission.invalidFormat'),
                'date': utils.getDate(),
                'code': codeValues.mission.jsonInvalid,
                'okText': vm.$t('cansat.notifications.modal.okBtn'),
                'uuid': utils.generateUUID().toString(),
                'type': vm.$t('cansat.notifications.center.types.error'),
                'cancelDisabled': true
            })
            return codeValues.mission.jsonInvalid
        }
        let file = JSON.parse(data)

        // Invalid project 
        if(file.mission == undefined || file.mission.data == undefined || file.mission.data.sensors == undefined || file.mission.data.sensors.length == 0 || file.mission.data.location == undefined){ 
            store.commit('pushNotificationModal',{ 
                'title': vm.$t('cansat.notifications.modal.mission.import'), 
                'content': vm.$t('cansat.notifications.modal.mission.invalidContent'),
                'date': utils.getDate(),
                'code': codeValues.mission.keysInvalid,
                'okText': vm.$t('cansat.notifications.modal.okBtn'),
                'uuid': utils.generateUUID().toString(),
                'type': vm.$t('cansat.notifications.center.types.error'),
                'cancelDisabled': true
            })
            return codeValues.mission.keysInvalid
        }else{

            let sensors = file.mission.data.sensors
            let location = file.mission.data.location

            // Missing keys
            let missingKeysSensors = sensors.filter(function(s,index) { 
                return (s._type == undefined || s.id == undefined || s.samples == undefined)
            })

            if(missingKeysSensors.length > 0 || location.history == undefined || location.history.length == undefined || location.history.length == 0){ 
                store.commit('pushNotificationModal',{ 
                    'title': vm.$t('cansat.notifications.modal.mission.import'), 
                    'content': vm.$t('cansat.notifications.modal.mission.incompleteFormat'),
                    'date': utils.getDate(),
                    'code': codeValues.mission.missingKeys,
                    'okText': vm.$t('cansat.notifications.modal.okBtn'),
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.error'),
                    'cancelDisabled': true
                })
                return codeValues.mission.missingKeys
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
                    'title': vm.$t('cansat.notifications.modal.mission.import'), 
                    'content': vm.$t('cansat.notifications.modal.mission.missingSensors'),
                    'date': utils.getDate(),
                    'code': codeValues.mission.missingSensors,
                    'okText': vm.$t('cansat.notifications.modal.okBtn'),
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.error'),
                    'cancelDisabled': true
                })
                return codeValues.mission.missingSensors
            }

            // Find duplicates ids
            let s = sensors.map(function(item){ return item.id });
            let isDuplicate = s.some(function(item, idx){ 
                return s.indexOf(item) != idx 
            });
            if(isDuplicate){
                store.commit('pushNotificationModal',{ 
                    'title': vm.$t('cansat.notifications.modal.mission.import'), 
                    'content': vm.$t('cansat.notifications.modal.mission.duplicateIds'),
                    'date': utils.getDate(),
                    'code': codeValues.mission.duplicateIDs,
                    'okText': vm.$t('cansat.notifications.modal.okBtn'),
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.error'),
                    'cancelDisabled': true
                })
                return codeValues.mission.duplicateIDs
            }

            // Find wrong IDs
            for(let s= 0 ;s < defaultSensors.sensors.length;s++){
                let matchId = sensors.filter(function(sensor,index) { 
                    return (sensor.id == defaultSensors.sensors[s].id && sensor._type == defaultSensors.sensors[s]._type)
                })
                if(matchId.length !=1){
                    store.commit('pushNotificationModal',{ 
                        'title': vm.$t('cansat.notifications.modal.mission.import'), 
                        'content': vm.$t('cansat.notifications.modal.mission.wrongID'),
                        'date': utils.getDate(),
                        'code': codeValues.mission.wrongIDs,
                        'okText': vm.$t('cansat.notifications.modal.okBtn'),
                        'uuid': utils.generateUUID().toString(),
                        'type': vm.$t('cansat.notifications.center.types.error'),
                        'cancelDisabled': true
                    })
                    return codeValues.mission.wrongIDs
                }
            }

            // Empty samples or history
            let lengthSamplesSensors = sensors.filter(function(sensor,index) { 
                return (sensor.samples.length != sensors[0].samples.length || sensor.samples.length == 0 ) 
            })
            if(lengthSamplesSensors.length != 0 || location.history.length == 0){
                store.commit('pushNotificationModal',{ 
                    'title': vm.$t('cansat.notifications.modal.mission.import'), 
                    'content': vm.$t('cansat.notifications.modal.mission.samplesError'),
                    'date': utils.getDate(),
                    'code': codeValues.mission.samplesError,
                    'okText': vm.$t('cansat.notifications.modal.okBtn'),
                    'uuid': utils.generateUUID().toString(),
                    'type': vm.$t('cansat.notifications.center.types.error'),
                    'cancelDisabled': true
                })
                return codeValues.mission.samplesError
            }

            // It's a valid project
            store.commit('pushNotificationModal',{ 
                'title': vm.$t('cansat.notifications.modal.mission.loadedOk'), 
                'date': utils.getDate(),
                'code': 0,
                'uuid': utils.generateUUID().toString(),
                'type': vm.$t('cansat.notifications.center.types.info')
            })
            store.commit('pushNotificationToast',{ 
                'text': vm.$t('cansat.notifications.modal.mission.loadedOk'), 
                'icon': 'fa-check'          
            })

            store.commit('createSensorMission',{
                clear: true
            })
            for(let s= 0; s< sensors.length; s++){
                store.commit('createSensorMission',{
                    id: sensors[s].id,
                    _type: sensors[s]._type
                })
                for(let sample = 0; sample < sensors[s].samples.length; sample++){
                    store.commit('addSensorSample',{
                        index: s,
                        timespan: utils.getDate(),
                        samples: {
                            ... (sensors[s].samples[sample].lastValue != undefined ? { lastValue: sensors[s].samples[sample].lastValue} : []),
                            ... (sensors[s].samples[sample].x != undefined ? { x: sensors[s].samples[sample].x} : []),
                            ... (sensors[s].samples[sample].y != undefined ? { y: sensors[s].samples[sample].y} : []),
                            ... (sensors[s].samples[sample].z != undefined ? { z: sensors[s].samples[sample].z} : []),
                            timespan: sensors[s].samples[sample].timespan
                        }
                    })
                }
            }

            store.commit('addLocationMission',{
                clear: true
            })
            for(let h= 0; h< location.history.length; h++){
                store.commit('addLocationMission',
                    {   
                        lat: location.history[h].lat,
                        lng: location.history[h].lng, 
                        timespan: utils.getDate()
                    }
                )
            }

            store.commit('setPathMission', {
                path: path
            })

            store.commit('setTestStatus',{ 
                cansatIndex: 0, 
                testOk: false
            })

            store.commit('setMissionType',{
                cansatIndex: 0,
                missionType: 'imported'
            })

            vm.$router.push({ name: 'dashboardMission' })
            return codeValues.project.ok
        }
    }
}
