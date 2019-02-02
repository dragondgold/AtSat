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
    project:{
      path:'',                // Project
      cansat:[{
        id: '',
        name:'',              // A friendly name for the cansat
        connected:'',         // is the cansat connected?
        location:{            // GPS data  
          lat: -31.416930,
          lng: -62.084470,    
          history:[]          // Location record
        },
        signal: 0,            // Signal level '%'    
        battery: 0,           // Battery level '%'
        altitude: 0,          // It´s a copy of altitude on sensor. Is needed for map.'m'
        testOk: false,        // State of test
        missionActive: false, // If it's false we need create a mission
        projectActive: false, // If it's false we need create a project
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
        name:'',          // Mission name
        createdDate: '',  
        endDate:'',
        data:{    // Data to import or export: 
                  // when it's imported we need load cansat 
                  // when it´s exported we take data from cansat

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
      ... (data.function != undefined ? {function: data.function} : [])
    })
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
    if(data.status != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].status = data.status
    if(data.lastValue != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].lastValue = data.lastValue
    if(data.minThreshold != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].minThreshold = data.minThreshold
    if(data.maxThreshold != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].maxThreshold = data.maxThreshold
    if(data.minValue != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].minValue = data.minValue
    if(data.maxValue != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].maxValue = data.maxValue
    if(data.x != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].x = data.x
    if(data.y != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].y = data.y
    if(data.z != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].z = data.z
    if(data.step != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].step = data.step
    if(data.unit != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].unit = data.unit
    if(data.type != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1].type = data.type
    if(data._type != undefined) state.axtec.project.cansat[data.cansatIndex].sensors[data.id-1]._type = data._type
  },
  addNewLocation(state,data){
    if(data.clear){
      state.axtec.project.cansat[data.cansatIndex].location.history = []
    }
    state.axtec.project.cansat[data.cansatIndex].location.history.push({
      lat: data.lat, 
      lng: data.lng,
      date: data.date,              
    }) 
    state.axtec.project.cansat[data.cansatIndex].location.lat = data.lat
    state.axtec.project.cansat[data.cansatIndex].location.lng = data.lng
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
}

export default {
  state,
  mutations,
  actions
}
