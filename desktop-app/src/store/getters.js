const toggleWithoutAnimation = state => state.app.sidebar.withoutAnimation
const config = state => state.app.config
const palette = state => state.app.config.palette
const isLoading = state => state.app.isLoading
const axtec = state => state.app.axtec
const location = state => state.app.axtec.project.cansat[0].location
const cansatSignal = state => state.app.axtec.project.cansat[0].signal
const cansatName = state => state.app.axtec.project.cansat[0].name
const cansatAltitude = state => state.app.axtec.project.cansat[0].altitude
const cansatBattery = state => state.app.axtec.project.cansat[0].battery
const cansatSensors = state => state.app.axtec.project.cansat[0].sensors

export {
  toggleWithoutAnimation,
  config,
  palette,
  isLoading,
  axtec,
  location,
  cansatSignal,
  cansatName,
  cansatAltitude,
  cansatBattery,
  cansatSensors
}
