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
    debug: true,
    notificationsModal: [
        
    ],
    notificationsToast: [
        
    ],
    project:{
      path:'',
      cansat:[{
        id: '',
        name:'',
        connected:'',
        location:{
          lat: -31.416930,
          lng: -62.084470,
          history:[]
        },
        signal: 0,
        battery: 0,
        altitude: 0, // It´s a copy of altitude on sensor. Is needed for map
        sensors:[ 
          {
            id: '',
            type: '',
            status:'',
            samples: [{
              value: '',
              timespan:''
            }],
            thresholdP: '',
            thresholdN: ''
          }
        ],
        actuators: [
          {
            type:'', // Parachute
            status: ''
          },
          {
            type:'', // Balloon
            status: ''
          }
        ],
      }],
      earthStation: {
        id: '',
        port: '',
        connected: false
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
      ... (data.cancelDisabled != undefined ? {cancelDisabled: data.cancelDisabled} : {cancelDisabled: false})
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
      ... (data.minValue != undefined ? { minValue: data.minValue} : []),
      ... (data.maxValue != undefined ? {maxValue: data.maxValue} : []),
      ... (data.lastValue != undefined ? {lastValue: data.lastValue} : []),
      ... (data.minThreshold != undefined ? {minThreshold: data.minThreshold} : []),
      ... (data.maxThreshold != undefined ? {maxThreshold: data.maxThreshold} : []),
      ... (data.lastValue != undefined ? {lastValue: data.lastValue} : []),
      ... (data.x != undefined ? {x: data.x} : []),
      ... (data.y != undefined ? {y: data.y} : []),
      ... (data.z != undefined ? {z: data.z} : []),
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
}

export default {
  state,
  mutations,
  actions
}
