<template>
    <div class="form-wizard-tab-content">
        <h4>{{ $t('cansat.resources.sensors.tabs.imu.title')}}</h4>
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
                    <td>{{sensor.x}}</td>
                    <td>{{sensor.y}}</td>
                    <td>{{sensor.z}}</td>
                    <td> 
                        <span class="input-group" >
                            <input class="input--textfield disable-caret" @keypress.prevent
                            :min="sensor.minValue" :max="sensor.maxValue" :value="sensor.maxThreshold" :step="sensor.step" type="number" @input="updateMaxThreshold(sensor,$event.target.value)"/>
                        </span>
                    </td>
                    <td>{{sensor.unit}}</td>
                    <td> 
                        <i class="fa icon-table" :class="[sensor.status.includes('ok') ? 'fa-check icon-table-success' : 'fa-exclamation-triangle icon-table-danger']" ></i> 
                    </td>
                    <td> 
                    <vuestic-tooltip :options="{ 'content': $t('cansat.resources.sensors.table.edit'), 'placement': 'right'  }">
                        </vuestic-tooltip> 
                        
                        <vuestic-tooltip :options="{ 'content': $t('cansat.resources.sensors.tabs.imu.table.resetThreshold'), 'placement': 'right'  }">
                            <div class="btn btn-info btn-with-icon btn-micro rounded-icon">
                                <div class="btn-with-icon-content" @click="resetThreshold(sensor)" >                                   
                                    <i  class="fa fa-refresh"></i>
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

export default {
    name: 'imu-tab',
    data () {
        return {
            fieldsSensors: [
                { "title" : 'cansat.resources.sensors.tabs.imu.table.id'},
                { "title" : 'cansat.resources.sensors.tabs.imu.table.type'}, 
                { "title": 'cansat.resources.sensors.tabs.imu.table.x'}, 
                { "title": 'cansat.resources.sensors.tabs.imu.table.y'},
                { "title": 'cansat.resources.sensors.tabs.imu.table.z'}, 
                { "title" : 'cansat.resources.sensors.tabs.imu.table.maxThreshold'},
                { "title" : 'cansat.resources.sensors.tabs.imu.table.unit'},
                { "title" : 'cansat.resources.sensors.tabs.imu.table.status'},
                { "title" : 'cansat.resources.sensors.tabs.imu.table.actions'}],
            sensors: this.$store.getters.axtec.project.cansat[0].sensors,
        }
    },

    methods:{
        filterSensors(s){
            let filtered = s.filter(function(n) {
                return (n.x != undefined)
            });
            return filtered
        },
        updateMaxThreshold(sensor,value){
            let s = JSON.parse(JSON.stringify(sensor)) // https://github.com/vuejs/vue/issues/971
            if(s.maxThreshold == '' || s.maxThreshold > s.maxValue){
                s.maxThreshold = s.maxValue
            }else{
                s.maxThreshold = Number(value)
            }
            this.$store.commit('setSensor', s)
        },
        resetThreshold(sensor){
            let s = JSON.parse(JSON.stringify(sensor)) 
            s.maxThreshold = s.maxValue;
            this.$store.commit('setSensor', s)
        }
    }
}
</script>

<style lang="scss">
    .icon-table {
        font-size: 1rem;
        color: white;
        border-radius: 50%;
        padding: 0.5rem
    }
    .icon-table-success{
        background-color: $brand-success;
    }
    .icon-table-info{
        background-color: $brand-info;
    }
    .icon-table-danger{
        background-color: $brand-danger;
    }

</style>