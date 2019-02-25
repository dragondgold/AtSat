const state = {
  sidebar: {
    opened: false,
    withoutAnimation: false
  },
  config: {
    googleMaps: {
      apiKey: 'AIzaSyBNAqPrTQoz9P4NBlDDyfxrnKiafkaL8iQ'
    },
    windowMatchSizeLg: '(min-width: 992px)',
    palette: {
      primary: '#4ae387',
      danger: '#e34a4a',
      info: '#4ab2e3',
      success: '#db76df',
      warning: '#f7cc36',
      white: '#fff',
      black: '#000',
      fontColor: '#34495e',
      transparent: 'transparent',
      lighterGray: '#ddd'
    }
  },
  isLoading: true,
  axtec: {
    debug: true,              // For debugging. 
    notificationsModal: [     // Notification array. If it's type is info we don't show modal but it's appears in notification center
        
    ],
    notificationsToast: [
        
    ],
    serialPorts: [],
    project:{
      path:'',                // Project
      cansat:[{
        id: '',
        name:'',              // A friendly name for the cansat
        connected:'',         // is the cansat connected?
        signal: 0,            // Signal level '%'    
        rssi: 0,
        lqi: 0,
        battery: 0,           // Battery level '%'
        altitude: 0,          // It´s a copy of altitude on sensor. Is needed for map.'m'
        testOk: false,        // State of test
        missionActive: false, // If it's false we need create a mission
        missionFinish: false, // It´s true when finish mission
        missionType: '',      // 'created' for new mission or 'imported' when we load a mission
        sensors:[ 
          {
            id: '',           // ID = CMD Sent and received from CanSat
            type: '',         // Showed in table. We need translate it
            status:'',        // If we get 'Ok' show 'fa-check' else we show 'fa-exclamation-triangle' in the table
            step: '',         // Minimum step when we change threshold
            minValue: '',     // Min Value allowed by sensor
            maxValue: '',     // Max Value allowed by sensor
            lastValue:'',     // Last value obtained from the CanSat of this specific sensor
            minThreshold: '', // Min threshold set by user
            maxThreshold:'',  // Max threshold set by user
            x: '',            // Last X value obtained from the CanSat of the vectorial sensor
            y: '',            // Last Y value obtained from the CanSat of the vectorial sensor
            z: '',            // Last Z value obtained from the CanSat of the vectorial sensor
            unit: '',         // Unit measured by the sensor
            cansatIndex:'',   // Default index for the selected CanSat
            _type: ''         // Used to apply filters: 
                              //  1) If it's 'vector' we get (x,y,z) to track it.
                              //  2) If it's 'scalar' we get only one value to track.
                              //  3) If it´s 'power' same as 'scalar' but only shown in the power supplies table 
                              //  4) If was created by user we get 'user'. Structure equals to 'scalar'.
          }
        ],
        actuators: [
          {
            cansatIndex: 0,
            actuatorIndex: 0,                             // 0 = parachute or 1 = balloon
            type:'cansat.resources.actuators.parachute',  // 'cansat.resources.actuators.parachute' or 'cansat.resources.actuators.balloon'
            status: 'cansat.resources.close'              // 'cansat.resources.close' or 'cansat.resources.open'
          },
          {
            cansatIndex: 0,
            actuatorIndex: 1,                             // 0 = parachute or 1 = balloon
            type:'cansat.resources.actuators.balloon',    // 'cansat.resources.actuators.parachute' or 'cansat.resources.actuators.balloon'
            status: 'cansat.resources.close'              // 'cansat.resources.close' or 'cansat.resources.open'
          }
        ],
      }],
      earthStation: {
        id: '',           
        port: '',         
        connected: false  
      },
      mission:{
        path: '',          // Path
        cansatName: '',
        startDate: '',  
        endDate:'',
        data:{            // Data to import or export: 
          sensors: [],               
          location: {
            lat: -31.416930,
            lng: -62.084470, 
            timespan: 0,
            history:[

            ]
          }                
        }
      }
    }
  }
}

const mutations = {
  setLoading (state, isLoading) {
    state.isLoading = isLoading
  },
  'TOGGLE_WITHOUT_ANIMATION' (state, value) {
    state.sidebar.withoutAnimation = value
  },
  axtecPath(state, path){
    state.axtec.project.path = path
  },
  pushNotificationModal(state,data){
    state.axtec.notificationsModal.push({
      ... (data.title != undefined ? { title: data.title} : []),
      ... (data.date != undefined ? {date: data.date} : []),
      ... (data.content != undefined ? {content: data.content} : []),
      ... (data.code != undefined ? {code: data.code} : []),
      ... (data.okCallback != undefined ? {okCallback: data.okCallback} : []),
      ... (data.cancelCallback != undefined ? {cancelCallback: data.cancelCallback} : []),
      ... (data.okText != undefined ? {okText: data.okText} : []),
      ... (data.cancelText != undefined ? {cancelText: data.cancelText} : []),
      ... (data.uuid != undefined ? {uuid: data.uuid} : []),
      ... (data.type != undefined ? {type: data.type} : []),
      ... (data.cancelDisabled != undefined ? {cancelDisabled: data.cancelDisabled} : {cancelDisabled: false}),
      ... (data.function != undefined ? {function: data.function} : []),
      show: false
    })
  },
  setShowModal(state,data){
    state.axtec.notificationsModal[data.index].show = data.show
  },
  pushNotificationToast(state,data){
    state.axtec.notificationsToast.push({
      'text': data.text, 
      'icon': data.icon,
    })
  },
  setStatusEarthStation(state, status){
    state.axtec.project.earthStation.connected = status
  },
  setStatusCanSat(state, data){
    state.axtec.project.cansat[data.index].connected = data.connected
  },
  setNameCanSat(state, data){
    state.axtec.project.cansat[data.index].name = data.name
  },
  setIDCanSat(state, data){
    state.axtec.project.cansat[data.index].id = data.id
  },
  setPortEarthStation(state, port){
    state.axtec.project.earthStation.port = port
  },
  setSignalCanSat(state, data){
    state.axtec.project.cansat[data.index].signal = data.signal
  },
  setActuators(state, data){
    if(data.status != undefined) state.axtec.project.cansat[data.cansatIndex].actuators[data.actuatorIndex].status = data.status
    if(data.type != undefined) state.axtec.project.cansat[data.cansatIndex].actuators[data.actuatorIndex].type = data.type
  },
  addNewSensor(state,data){
    if(data.clear){
      state.axtec.project.cansat[data.cansatIndex].sensors = []
    }
    state.axtec.project.cansat[data.cansatIndex].sensors.push({
      id: data.id, 
      type: data.type,
      status: data.status,
      step: data.step,
      ... (data.minValue != undefined ? { minValue: data.minValue} : { minValue: 0}),
      ... (data.maxValue != undefined ? { maxValue: data.maxValue} : []),
      ... (data.lastValue != undefined ? { lastValue: data.lastValue} : []),
      ... (data.minThreshold != undefined ? { minThreshold: data.minThreshold} : { minThreshold: 0}),
      ... (data.maxThreshold != undefined ? { maxThreshold: data.maxThreshold} : []),
      ... (data.lastValue != undefined ? { lastValue: data.lastValue} : { lastValue: 0 }),
      ... (data.x != undefined ? { x: data.x} : { x: 0}),
      ... (data.y != undefined ? { y: data.y} : { y: 0}),
      ... (data.z != undefined ? { z: data.z} : { z: 0}),
      unit: data.unit,
      cansatIndex: data.cansatIndex, 
      _type: data._type           
    }) 
  },
  setSensor(state,data){
    if(data.status != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].status = data.status
    if(data.lastValue != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].lastValue = data.lastValue
    if(data.minThreshold != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].minThreshold = data.minThreshold
    if(data.maxThreshold != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].maxThreshold = data.maxThreshold
    if(data.minValue != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].minValue = data.minValue
    if(data.maxValue != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].maxValue = data.maxValue
    if(data.x != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].x = data.x
    if(data.y != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].y = data.y
    if(data.z != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].z = data.z
    if(data.step != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].step = data.step
    if(data.unit != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].unit = data.unit
    if(data.type != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex].type = data.type
    if(data._type != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex]._type = data._type

    let sensor =  state.axtec.project.cansat[data.cansatIndex].sensors[data.sensorIndex];
    if(sensor.type != undefined && sensor.type.includes('altitude')){
      state.axtec.project.cansat[0].altitude = data.lastValue
    }
  },
  addNewLocation(state,data){
    if(data.clear){
      state.axtec.project.mission.data.location.history = []
    }
    state.axtec.project.mission.data.location.history.push({
      lat: data.lat, 
      lng: data.lng,
      timespan: data.timespan,              
    }) 
    state.axtec.project.mission.data.location.lat = data.lat
    state.axtec.project.mission.data.location.lng = data.lng
    state.axtec.project.mission.data.location.ln = data.timespan
  },
  removeSensor(state,data){
    state.axtec.project.cansat[data.cansatIndex].sensors.splice(data.id-1, 1)
  },
  setTestStatus(state,data){
    state.axtec.project.cansat[data.cansatIndex].testOk = data.testOk
  },
  setMissionStatus(state,data){
    state.axtec.project.cansat[data.cansatIndex].missionActive = data.missionActive
  },
  setProjectStatus(state,data){
    state.axtec.project.cansat[data.cansatIndex].projectActive = data.projectActive
  },
  setPathMission(state,data){
    state.axtec.project.mission.path = data.path
  },
  setMissionType(state,data){
    state.axtec.project.cansat[data.cansatIndex].missionType = data.missionType // 'created' or 'imported'
  },
  createSensorMission(state,data){
    if(data.clear){
      state.axtec.project.mission.data.sensors = []
    }else{
      state.axtec.project.mission.data.sensors.push({
        id: data.id, 
        _type: data._type ,
        type: data.type,
        unit: data.unit,
        minValue: data.minValue,
        maxValue: data.maxValue,
        samples: []     
      }) 
    }
  },
  addLocationMission(state,data){
    if(data.clear){
      state.axtec.project.mission.data.location.history = []
    }else{
      state.axtec.project.mission.data.location.history.push({
        lat: data.lat,   
        lng: data.lng,
        timespan: data.timespan   
      }) 
      state.axtec.project.mission.data.location.lat = data.lat 
      state.axtec.project.mission.data.location.lng = data.lng
      state.axtec.project.mission.data.location.timespan = data.timespan
    }
  },
  addSensorSample(state,data){
    if(state.axtec.project.cansat[0].missionActive){
      try {
        if(data.index >= state.axtec.project.mission.data.sensors.length){
          debugger
        }else{
          state.axtec.project.mission.data.sensors[data.index].samples.push({
            ... (data.samples.lastValue != undefined ? { lastValue: data.samples.lastValue} : []),
            ... (data.samples.x != undefined ? { x: data.samples.x} : []),
            ... (data.samples.y != undefined ? { y: data.samples.y} : []),
            ... (data.samples.z != undefined ? { z: data.samples.z} : []),
            timespan: data.samples.timespan              
          }) 
        }
        
      } catch (error) {
        debugger
      }    
    }
  },
  setDateMission(state,data){
    if(data.startDate != undefined) state.axtec.project.mission.startDate = data.startDate
    if(data.endDate != undefined) state.axtec.project.mission.endDate = data.endDate           
  },
  setCansatMission(state,data){
    if(data.name != undefined) state.axtec.project.mission.cansatName = data.name          
  },
  setFinishMission(state,data){
    if(data.finish != undefined) state.axtec.project.cansat[0].missionFinish = data.finish          
  },
  setBatteryLevel(state,data){
    if(data.level != undefined) state.axtec.project.cansat[0].battery = data.level          
  },
  loadMission(state,data){
    state.axtec.project.mission = data.mission
  },
  setRSSI(state,data){
    state.axtec.project.cansat[0].rssi = data.rssi
  },
  setLQI(state,data){
    state.axtec.project.cansat[0].lqi = data.lqi
  },
  setSerialPorts(state,data){
    state.axtec.serialPorts = data.ports
  },
  setPowerSuppliesState(state,data){
    for(let s = 0 ; s < state.axtec.project.cansat[0].sensors.length; s++){
      if(state.axtec.project.cansat[0].sensors[s]._type == 'power'){
        state.axtec.project.cansat[0].sensors[s].status = data.status
      }
    }
  }
}

const actions = {
  isToggleWithoutAnimation ({ commit }, value) {
    commit('TOGGLE_WITHOUT_ANIMATION', value)
  },
  axtecPath( { commit }, value){
    commit(axtecPath,value)
  },
  pushNotificationModal( { commit }, data){
    commit(pushNotificationModal,data)
  },
  setShowModal(state,data){
    commit(setShowModal,data)
  },
  pushNotificationToast( { commit }, data){
    commit(pushNotificationToast,data)
  },
  setStatusEarthStation({ commit }, data){
    commit(setStatusEarthStation,data)
  },
  setStatusCanSat({ commit }, data){
    commit(setStatusCanSat,data)
  },
  setNameCanSat({ commit }, data){
    commit(setNameCanSat,data)
  },
  setIDCanSat({ commit }, data){
    commit(setIDCanSat,data)
  },
  setSignalCanSat({ commit }, data){
    commit(setSignalCanSat,data)
  },
  setPortEarthStation({ commit }, data){
    commit(setPortEarthStation,data)
  },
  setActuators({ commit }, data){
    commit(setActuators,data)
  },
  setActuators({ commit }, data){
    commit(setActuators,data)
  },
  addNewSensor({ commit }, data){
    commit(addNewSensor,data)
  },
  setSensor({ commit }, data){
    commit(setSensor,data)
  },
  addNewLocation({ commit }, data){
    commit(addNewLocation,data)
  },
  removeSensor({ commit }, data){
    commit(removeSensor,data)
  },
  setTestStatus({ commit }, data){
    commit(setTestStatus,data)
  },
  setMissionStatus({ commit }, data){
    commit(setMissionStatus,data)
  },
  setProjectStatus({ commit }, data){
    commit(setProjectStatus,data)
  },
  setPathMission({ commit }, data){
    commit(setPathMission,data)
  },
  setMissionType({ commit }, data){
    commit(setMissionType,data)
  },
  createSensorMission({ commit }, data){
    commit(createSensorMission,data)
  },
  addLocationMission({ commit }, data){
    commit(addLocationMission,data)
  },
  addSensorSample({ commit }, data){
    commit(addSensorSample,data)
  },
  setDateMission({ commit }, data){
    commit(setDateMission,data)
  },
  setCansatMission({ commit }, data){
    commit(setCansatMission,data)
  },    
  setFinishMission({ commit }, data){
    commit(setFinishMission,data)
  },    
  setBatteryLevel({ commit }, data){
    commit(setBatteryLevel,data)
  },
  loadMission({ commit }, data){
    commit(loadMission,data)
  },
  setSerialPorts({ commit }, data){
    commit(setSerialPorts,data)
  },
  setPowerSuppliesState({ commit }, data){
    commit(setPowerSuppliesState,data)
  },
}

export default {
  state,
  mutations,
  actions
}
