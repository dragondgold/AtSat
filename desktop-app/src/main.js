// Polyfills
// import 'es6-promise/auto'
// import 'babel-polyfill'

// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import VeeValidate from 'vee-validate'
import App from './App'
import store from './store'
import router from './router'
import VuesticPlugin from '@/vuestic-theme/vuestic-plugin'
import './i18n'
import YmapPlugin from 'vue-yandex-maps'
import CanSatAPI from './services/CanSatAPI'

const { ipcRenderer  } = require('electron')

ipcRenderer.on('close', (event) => {
  CanSatAPI.killWorker()
  ipcRenderer.send('quit')
})

Vue.use(VuesticPlugin)
Vue.use(YmapPlugin)

// NOTE: workaround for VeeValidate + vuetable-2
Vue.use(VeeValidate, { fieldsBagName: 'formFields' })
Vue.config.devtools = true

router.beforeEach((to, from, next) => {
  if(to.name != 'not-found-simple'){ // Avoiding router
    store.commit('setLoading', true)
    next()
  }else{

  }
})

router.afterEach((to, from) => {
  store.commit('setLoading', false)
})

/* eslint-disable no-new */



let vm = new Vue({
  el: '#app',
  router,
  store,
  render: h => h(App)
})

global.vm = vm

CanSatAPI.initWorker()