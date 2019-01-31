<template>
    <div class="form-wizard-tab-content"> 
        <div v-if="!addSensorHere" class="col-md-12">
            <h4>{{ $t('cansat.resources.sensors.tabs.createdByUser.title')}}</h4>
            <span v-if="filterSensors(filtered).length == 0">
            {{$t('cansat.resources.sensors.tabs.createdByUser.empty')}}
            </span> 
            <p class="mb-3">
                <button class="btn btn-micro float-left btn-success" @click="goToNewSensor()">
                {{ $t('cansat.resources.sensors.tabs.createdByUser.button') }}
                <span class="glyphicon glyphicon-arrow-right"></span>
                </button>
            </p>  
         </div>
         <div v-else class="col-md-12 form-wizard-tab-content">
            <new-sensor-form ref="newSensorForm"/>
         </div>
        <table v-if="filterSensors(filtered).length > 0 || addSensorHere" class="table table-striped first-td-padding col-md-12">
            <thead>
                <tr>
                    <td v-for="(field,key) in fieldsSensors" :key="key">
                    {{$t(field.title)}}
                    </td>             
                </tr>
            </thead>
            <tbody>
                <tr v-for="(sensor,key) in filtered" :key="key">
                    <td>{{sensor.id}}</td>
                    <td>{{$t(sensor.type)}}</td>
                    <td> 
                        <span class="input-group" >
                            <input class="input--textfield disable-caret" :disabled="addSensorHere"
                            :min="sensor.minValue" :max="sensor.maxThreshold" :value="sensor.minThreshold" :step="sensor.step" type="number" @input="updateMinThreshold(sensor,$event.target.value)"/>
                        </span>
                    </td>
                    <td>{{sensor.lastValue}}</td>
                    <td>       
                       <span class="input-group" >
                            <input class="input--textfield disable-caret" :disabled="addSensorHere"
                            :min="sensor.minThreshold" :max="sensor.maxValue" :value="sensor.maxThreshold" :step="sensor.step" type="number" @input="updateMaxThreshold(sensor,$event.target.value)"/>
                        </span>
                        
                    </td>
                    <td>{{sensor.unit}}</td>
                    <td> 
                        <i class="fa icon-table" :class="[sensor.status.includes('ok') ? 'fa-check icon-table-success' : 'fa-exclamation-triangle icon-table-danger']" ></i> 
                    </td>
                    <td> 
                        <vuestic-tooltip v-if="!addSensorHere" :options="{ 'content': $t('cansat.resources.sensors.tabs.variables.table.resetThreshold'), 'placement': 'right'  }">
                            <div v-if="sensor.status !='Active'" class="btn btn-info btn-with-icon btn-micro rounded-icon">
                                <div class="btn-with-icon-content" @click="resetThreshold(sensor)" >                                   
                                    <i  class="fa fa-refresh"></i>
                                </div>
                            </div>
                        </vuestic-tooltip> 
                        <div v-else> 
                            <vuestic-tooltip  :options="{ 'content': $t('cansat.resources.sensors.tabs.createdByUser.newSensor.edit'), 'placement': 'right'  }">
                                <div class="btn btn-info btn-with-icon btn-micro rounded-icon">
                                    <div class="btn-with-icon-content" @click="editSensor(sensor)" >                                   
                                        <i class="fa fa-edit"></i>
                                    </div>
                                </div>
                            </vuestic-tooltip> 
                            <vuestic-tooltip v-if="filtered.length == (key+1)"  :options="{ 'content': $t('cansat.resources.sensors.tabs.createdByUser.newSensor.delete'), 'placement': 'right'  }">
                                <div class="btn btn-danger btn-micro rounded-icon btn-with-icon">
                                    <div class="btn-with-icon-content" @click="removeSensor(sensor)" >                                   
                                        <i  class="fa fa-remove"></i>
                                    </div>
                                </div>
                            </vuestic-tooltip> 
                        </div>
                           
                    </td>
                </tr>
            </tbody> 
        </table>
    </div>
</template>

<script>
import newSensorForm from './elements/newSensorForm'
import defaultSensors from 'data/Sensors'
import { mapGetters } from 'vuex'

export default {
    name: 'add-sensor-widget',
    props: {
        addSensorHere: {
            type: Boolean,
            default: false
        }
    },
    components:{
        newSensorForm
    },
    computed:{
        ...mapGetters({
            sensors: 'cansatSensors',
        }),
    },
    watch:{
        sensors: {
            handler: function(changes) {
                this.filtered =  this.filterSensors(changes)
            },
            deep: true
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
            filtered: this.filterSensors(JSON.parse(JSON.stringify(this.$store.getters.axtec.project.cansat[0].sensors)))
        }
    },
    methods:{
        removeSensor(s){
            this.$refs.newSensorForm.removeSensor(s)
        },
        editSensor(s){
            this.$refs.newSensorForm.editSensor( JSON.parse(JSON.stringify(s)))
        },
        goToNewSensor(){
            this.$router.push({name:'newSensor'})
        },
        filterSensors(s){
            let filtered = s.filter(function(n,index) {
                return (n._type == 'user')
            });
            return filtered
        },
        updateMinThreshold(sensor,value){
            let s = JSON.parse(JSON.stringify(sensor)) // https://github.com/vuejs/vue/issues/971
            s.minThreshold = Number(value)
            this.$store.commit('setSensor', s)
        },
        updateMaxThreshold(sensor,value){
            let s = JSON.parse(JSON.stringify(sensor)) 
            s.maxThreshold = Number(value)
            this.$store.commit('setSensor', s)
        },
        resetThreshold(sensor){
            let s = JSON.parse(JSON.stringify(sensor)) 
            s.minThreshold = s.minValue;
            s.maxThreshold = s.maxValue;
            this.$store.commit('setSensor', s)
        }
    }
}
</script>