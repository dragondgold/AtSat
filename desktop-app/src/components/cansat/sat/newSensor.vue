<template>
  <div class="form-wizard-page">
      
    <div class="row">
      <div class="col-md-12">
        <vuestic-widget :headerText="$t('cansat.resources.sensors.tabs.createdByUser.newSensor.title')" class="pt-0">
            <vuestic-tooltip :options="{ 'content': $t('cansat.resources.sensors.tabs.createdByUser.newSensor.back'), 'placement': 'top'  }">
                <div class="btn btn-info btn-with-icon btn-micro rounded-icon">
                    <div class="btn-with-icon-content" @click="goBack()" >                                   
                        <i  class="fa fa-arrow-circle-o-left"></i>
                    </div>
                </div>
            </vuestic-tooltip> 
            <br>  
            <add-sensor-widget :addSensorHere="true"/>
        </vuestic-widget>
      </div>
    </div>
  </div>
</template>

<script>
import addSensorWidget from './Widgets/addSensorWidget'

import defaultActuators from 'data/Actuators'
import defaultSensors from 'data/Sensors'

export default {
    name: 'sensors-actuators-sat',
    components:{
        addSensorWidget
    },
    data () {
        return {

        }
    },
    methods:{
        goBack(){
            this.$router.go(-1)
        },
        goToLink(){
            this.$router.push({name:'linkSat'})
        }
    }
}
</script>