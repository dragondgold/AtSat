<template>
    <div class="form-wizard-tab-content">
        <table class="table table-striped first-td-padding col-md-12">
            <thead>
                <tr>
                    <td v-for="(field,key) in fieldsActuators" :key="key">{{$t(field.title)}}</td>             
                </tr>
            </thead>
            <tbody>
                <tr v-for="(actuator,key) in actuators" :key="key">
                    <td>{{$t(actuator.type)}}</td>
                    <td>{{$t(actuator.status)}}</td>
                    <td> 
                        <vuestic-tooltip v-if="actuator.status.includes('open')" :options="{ 'content': $t('cansat.resources.close'), 'placement': 'left'  }">
                            <div  class="btn btn-info btn-with-icon btn-micro rounded-icon">
                                <div class="btn-with-icon-content" @click="closeActuator(key)">                                   
                                    <i  class="ion ion-md-close-circle"></i>
                                </div>
                            </div>
                        </vuestic-tooltip>  
                        <vuestic-tooltip v-else :options="{ 'content': $t('cansat.resources.open'), 'placement': 'right'  }">
                            <div  class="btn btn-info btn-with-icon btn-micro rounded-icon">
                                <div class="btn-with-icon-content" @click="openActuator(key)">                                   
                                    <i  class="ion ion-md-open"></i>
                                </div>
                            </div>
                        </vuestic-tooltip>                          
                    </td>
                </tr>
            </tbody> 
        </table>
        <p v-if="powerGood" class="pt-1 mb-3" style="text-align:center">
            <button class="btn btn-success btn-micro" @click="goToSensors()">
                {{'cansat.protections.goToSensors' | translate }}
            </button>
        </p>
    </div>
</template>

<script>
import actuators from 'data/Actuators'

export default {
    name: 'actuators-widget',
    data () {
        return {
            fieldsActuators: [
                { "title" : 'cansat.resources.actuators.table.type'}, 
                { "title": 'cansat.resources.actuators.table.status'},
                { "title" : 'cansat.resources.actuators.table.actions'}], 
            actuators: this.$store.getters.axtec.project.cansat[0].actuators,
            powerGood: false
        }
    },
    created(){
         

    },

    computed: {
        getActuators(){
            return this.$store.getters.axtec.project.cansat[0].actuators
        }
    },

    watch:{
        getPowerSuplies(changes){
            this.powerSuplies = changes
        }
    },

    methods:{
        openActuator(key){
            this.$store.commit('setActuators',
            { 
                'cansatIndex': 0, 
                'actuatorIndex': key, 
                'status' : 'cansat.resources.open'
            })
        },
        closeActuator(key){
            this.$store.commit('setActuators',
            { 
                'cansatIndex': 0, 
                'actuatorIndex': key, 
                'status' : 'cansat.resources.close'
            })
        }
    }
}
</script>