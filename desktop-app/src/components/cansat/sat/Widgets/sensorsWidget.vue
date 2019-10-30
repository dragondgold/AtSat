<template>
    <div class="form-wizard-tab-content">
        <imu-tab/>
        <variables-tab :filterTableByType="'scalar'"/>
        <add-sensor-widget :addSensorHere="false" />
    </div>
</template>

<script>
import imuTab from './imuTab'
import variablesTab from './variablesTab'
import addSensorWidget from './addSensorWidget'

export default {
    name: 'sensors-widget',
    components:{
        imuTab,
        variablesTab,
        addSensorWidget
    },
}
</script>