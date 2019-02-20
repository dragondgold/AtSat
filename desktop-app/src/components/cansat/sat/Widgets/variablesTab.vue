<template>
    <div class="form-wizard-tab-content">
        <h4 v-if="filterTableByType!= 'power'">{{ $t('cansat.resources.sensors.tabs.variables.title')}}</h4>
        <table class="table table-striped first-td-padding col-md-12">
            <thead>
                <tr>
                    <td v-for="(field,key) in fieldsSensors" :key="key">
                    {{$t(field.title)}}
                    </td>             
                </tr>
            </thead>
            <tbody>
                <tr v-for="(sensor,key) in filterSensors(sensors)" :key="key">
                    <td>{{sensor.id}}</td>
                    <td>{{$t(sensor.type)}}</td>
                    <td> 
                        <span class="input-group" > 
                            <input class="input--textfield disable-caret" @keypress.prevent 
                            :min="sensor.minValue" :max="sensor.maxThreshold" :value="sensor.minThreshold" :step="sensor.step" type="number" @input="updateMinThreshold(sensor,$event.target.value)"/>
                        </span>
                    </td>
                    <td>{{sensor.lastValue}}</td>
                     <td>                        
                       <span class="input-group" >
                            <input class="input--textfield disable-caret" @keypress.prevent
                            :min="sensor.minThreshold" :max="sensor.maxValue" :value="sensor.maxThreshold" :step="sensor.step" type="number" @input="updateMaxThreshold(sensor,$event.target.value)"/>
                        </span>                   
                    </td>
                    <td>{{sensor.unit}}</td>
                    <td> 
                        <i class="fa icon-table" :class="[sensor.status.includes('ok') ? 'fa-check icon-table-success' : 'fa-exclamation-triangle icon-table-danger']" ></i> 
                    </td>
                    <td> 
                        <vuestic-tooltip :options="{ 'content': $t('cansat.resources.sensors.tabs.variables.table.resetThreshold'), 'placement': 'right'  }">
                            <div class="btn btn-info btn-with-icon btn-micro rounded-icon">
                                <div class="btn-with-icon-content" @click="resetThreshold(sensor)" >                                   
                                    <i  class="fa fa-refresh"></i>
                                </div>
                            </div>
                        </vuestic-tooltip>  
                        <vuestic-tooltip v-if="filterTableByType=='power' && !sensor.status.includes('ok')" :options="{ 'content': $t('cansat.resources.sensors.tabs.variables.table.resetPowerSuply'), 'placement': 'right'  }">
                            <div class="btn btn-success btn-with-icon btn-micro rounded-icon">
                                <div class="btn-with-icon-content" @click="resetPowerSuply()" >                                   
                                    <i  class="fa fa-wrench"></i>
                                </div>
                            </div>
                        </vuestic-tooltip>   
                    </td>
                </tr>
            </tbody> 
        </table>
    </div>
</template>

<script>
import sensors from 'data/Sensors'
import CanSatAPI from 'services/CanSatAPI'

export default {
    name: 'variables-tab',
    props: {
        filterTableByType: {
            type: String,
            required: true
        }
    },
    data () {
        return {
            fieldsSensors: [
                { "title" : 'cansat.resources.sensors.tabs.variables.table.id'},
                { "title" : 'cansat.resources.sensors.tabs.variables.table.type'}, 
                { "title": 'cansat.resources.sensors.tabs.variables.table.min'}, 
                { "title": 'cansat.resources.sensors.tabs.variables.table.value'},
                { "title": 'cansat.resources.sensors.tabs.variables.table.max'}, 
                { "title" : 'cansat.resources.sensors.tabs.variables.table.unit'},
                { "title" : 'cansat.resources.sensors.tabs.variables.table.status'},
                { "title" : 'cansat.resources.sensors.tabs.variables.table.actions'}],
            sensors: this.$store.getters.axtec.project.cansat[0].sensors,
        }
    },

    methods:{
        filterSensors(s){
            let filter = this.filterTableByType
            let filtered = s.filter(function(n) {
                return (n._type == filter)
            });
            return filtered
        },
        updateMinThreshold(sensor,value){
            let s = JSON.parse(JSON.stringify(sensor)) // https://github.com/vuejs/vue/issues/971
            if(s.minThreshold == '' || s.minThreshold < s.minValue){
                s.minThreshold = s.minValue
            }else{
                s.minThreshold = Number(value)
            }
            this.$store.commit('setSensor', s)
        },
        updateMaxThreshold(sensor,value){
            let s = JSON.parse(JSON.stringify(sensor)) 
            if(s.maxThreshold == '' || s.maxThreshold > s.maxValue){
                s.maxThreshold = s.maxValue
            }else{
                s.maxThreshold = Number(value)
            }
            this.$store.commit('setSensor', s)
        },
        resetThreshold(sensor){
            let s = JSON.parse(JSON.stringify(sensor)) 
            s.minThreshold = s.minValue
            s.maxThreshold = s.maxValue
            s.sensorIndex = s.id - 1
            this.$store.commit('setSensor', s)
        },
        resetPowerSuply(){
            let sensor = JSON.parse(JSON.stringify(this.sensors))
            let filtered = sensor.filter(function(n) {
                return (n._type == 'power')
            })

            

            for(let s = 0 ;s < filtered.length;s++){
                let sensor;
                let index = -1
                
                let searchID = this.$store.getters.axtec.project.mission.data.sensors
                for(let c = 0; c <searchID.length; c++ ){
                    if(searchID[c].id == filtered[s].sensorID){
                        index = c
                        sensor = searchID[c];
                        break;
                    }
                }
                if(sensor != undefined){ 
                    filtered[s].status= 'ok'
                    filtered[s].sensorID = index
                    this.$store.commit('setSensor', filtered[s])
                }
                CanSatAPI.enablePowerSupply()
            }
        }
    }
}
</script>
