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
        signal: '',
        sensors: [{
          type: '',
          status:'',
          samplingTime: '',
          samples: [{
            value: '',
            timespan:''
          }],
          threshold: ''
        }],
        actuators: [{
          type:'',
          status: ''
        }],
        protections: {
          powerSupply: [{
            name: 'cansat.protections.vBatt', // V Battery
            voltage: '',
            current: '',
            status: '',
            maxCurrent: '',
          },
          {
            name: '3.3 V Ext', // 3.3 V 
            voltage: '',
            current: '',
            status: '',
            maxCurrent: '',
          },
          { 
            name: '5 V Ext', // 5 V 
            voltage: '',
            current: '',
            status: '',
            maxCurrent: '',
          }]
        }
      }],
      earthStation: {
        id: '',
        port: '',
        connected: false
      }
    },
    data:{

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
      'title': data.title, 
      'date': data.date,
      'content': data.content,
      'code': data.code,
      'ok': data.okCallback,
      'cancel': data.cancelCallback,
      'okText': data.okText,
      'cancelText': data.cancelText,
      'uuid': data.uuid,
      'type': data.type,
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
  setElectricalProtectionsPS(state, data){ // Set data to an specific PS (Power supply)
    if(data.voltage != undefined) state.axtec.project.cansat[data.cansatIndex].protections.powerSupply[data.psIndex].voltage = data.voltage
    if(data.current != undefined) state.axtec.project.cansat[data.cansatIndex].protections.powerSupply[data.psIndex].current = data.current
    if(data.maxCurrent != undefined) state.axtec.project.cansat[data.cansatIndex].protections.powerSupply[data.psIndex].maxCurrent = data.maxCurrent
    if(data.status != undefined) state.axtec.project.cansat[data.cansatIndex].protections.powerSupply[data.psIndex].status = data.status
  },
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
  setElectricalProtectionsPS({ commit }, data){
    commit(setElectricalProtectionsPS,data)
  },
}

export default {
  state,
  mutations,
  actions
}
