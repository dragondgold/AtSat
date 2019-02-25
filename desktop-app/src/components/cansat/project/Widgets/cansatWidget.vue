<template>
    <div class="form-wizard-tab-content">
        <div v-if="ETConnected" style="text-align:center">
            <h4 v-if="enableWizard">{{'cansat.project.new.wizard.stepThree.description' | translate}}</h4>
            <h4 v-else>{{'cansat.link.tabs.cansat' | translate}}</h4>
            <div v-if="!isCanSatConnect">
                <div v-if="!disableIDList">
                    <p>{{ $t('cansat.project.new.wizard.stepThree.titleCanSat') }}</p>    
                    <vuestic-simple-select
                    :label="'cansat.project.new.wizard.stepThree.selectCanSat' | translate"
                    v-model="selectedCansat"
                    name="selectedCansat"
                    :required="true"
                    ref="selectedCansat"
                    v-bind:options="canSatList" disabled>
                    </vuestic-simple-select>
                </div>
               
                <p>{{ $t('cansat.project.new.wizard.stepThree.titleName') }}</p>    
                <div class="form-group with-icon-right" style="width: inherit"
                    :class="{'has-error': !isFormPathValid(canSatName), 'valid': isFormPathValid(canSatName)}">
                    <div class="input-group">
                        <input 
                        name="canSatName"
                        data-vv-as="Name"
                        v-model="canSatName"
                        v-validate="'required'"
                        required title="" 
                        @input="validateConnection"/>
                        <i
                        class="fa fa-exclamation-triangle error-icon icon-right input-icon"></i>
                        <i class="fa fa-check valid-icon icon-right input-icon"></i>
                        <label class="control-label">
                        {{$t('cansat.project.new.wizard.stepOne.name')}}
                        </label>
                        <i class="bar"></i>
                        <small v-show="!isFormPathValid(canSatName)" class="help text-danger">
                        {{$t('cansat.project.new.wizard.stepOne.invalidName')}}
                        </small>
                    </div>
                </div>
            </div>
            <div v-else>
                <table class="table table-striped first-td-padding col-md-12">
                    <thead>
                        <tr>
                            <td v-for="(field,key) in fields" :key="key">{{$t(field.title)}}</td>             
                        </tr>
                    </thead>
                    <tbody>
                        <tr v-for="(cansat,key) in cansats" :key="key">
                            <td>{{cansat.name}}</td>
                            <td>{{rssi}}</td>
                            <td>{{lqi}}</td>
                            <td>{{battery}}</td>

                            <td>
                                <div class="btn btn-danger btn-with-icon btn-micro rounded-icon">
                                    <div class="btn-with-icon-content" @click="disconnect()">
                                        <i class="fa fa-unlink"></i>
                                    </div>
                                </div>   
                            </td>
                        </tr>
                    </tbody>
                </table>
            </div>

            <div class="col-md-12" v-if="!enableWizard">         
                <div class="pt-1 mb-3" style="text-align:center">
                    <button v-if="!isCanSatConnect" class="btn btn-success btn-micro" @click="connect()">
                        {{'cansat.link.connect' | translate }}
                        <span class="fa fa-link"></span>
                    </button>
                </div>
            </div>
        </div>
        <div v-else style="text-align:center">
            <h4>{{'cansat.link.tabs.cansat' | translate}}</h4>
            <p>{{'cansat.link.etDisconnectedLabel' | translate}}</p>
        </div>
    </div>
</template>

<script>
const isValidPath = require('is-valid-path')
import CanSatAPI from 'services/CanSatAPI'
import defaultActuators from 'data/Actuators'
import defaultSensors from 'data/Sensors'
import utils from 'services/utils'

export default {
    name: 'cansat-widget',
    props: {
        enableWizard: {
            type: Boolean,
            default: true
        }
    },
    data () {
        return {
            canSatName: this.$store.getters.axtec.project.cansat[0].name,
            canSatList: ['id_mcp2210'],
            selectedCansat: this.$store.getters.axtec.project.cansat[0].id,
            valid: false,
            isCanSatConnect: this.$store.getters.axtec.project.cansat[0].connected,
            fields: [ {'title': 'cansat.link.table.name'}, {'title':'RSSI'},{'title':'LQI'}, {'title':'Batt'},{'title':'cansat.link.table.action'}],
            cansats: [this.$store.getters.axtec.project.cansat[0]],
            disableIDList: true // TODO: CREATE A LIST OF CANSAT DETECTED
        }
    },    
    created(){

    },
    computed:{
        rssi(){
            return this.$store.getters.axtec.project.cansat[0].rssi
        },
        lqi(){
            return this.$store.getters.axtec.project.cansat[0].lqi
        },
        battery(){
            return this.$store.getters.axtec.project.cansat[0].battery
        },
        ETConnected(){
            return this.$store.getters.axtec.project.earthStation.connected
        },
        CanSatConnected(){
            return this.$store.getters.axtec.project.cansat[0].connected
        },
        getCanSatName(){
            return this.$store.getters.axtec.project.cansat[0].name
        },
        getSelectedCanSat(){
            return this.$store.getters.axtec.project.cansat[0].id
        }
    },
    watch:{
        getCanSatName(name){
            this.canSatName = name
        },
        getSelectedCanSat(id){
            this.selectedCansat = id
        },
        CanSatConnected(connected){
            this.isCanSatConnect = connected
        },
    },
    methods:{
        setStatusesOnConnect(){
            this.$store.commit('setStatusCanSat', { 'index': 0, 'connected': true})
            this.$store.commit('setIDCanSat', { 'index': 0, 'id': this.selectedCansat})
            this.$store.commit('setNameCanSat', { 'index': 0, 'name': this.canSatName})
            this.$store.commit('setSignalCanSat', { 'index': 0, 'signal': this.signalLevel})      
        },
        clearStatusesOnDisconnect(){
            this.$store.commit('setStatusCanSat', { 'index': 0, 'connected': false})
            this.$store.commit('setIDCanSat', { 'index': 0, 'id': ''})
            this.$store.commit('setNameCanSat', { 'index': 0, 'name': ''})
            this.$store.commit('setSignalCanSat', { 'index': 0, 'signal': ''})
            this.resetSensors()
            this.resetActuators() 
        },
        connect(){
            if( this.canSatName != '' &&  (this.selectedCansat != '' || this.disableIDList)){
                this.setStatusesOnConnect()
                CanSatAPI.connectToCansat(this.connectOK, this.connectFAIL)
            }
        },
        connectOK(){
            this.$store.commit('setStatusCanSat', { 'index': 0, 'connected': true})
            this.$store.commit('pushNotificationToast',{ 
                'text': vm.$t('cansat.notifications.etCansatConnected'), 
                'icon': 'fa-check'          
            })
            this.$store.commit('pushNotificationModal',{ 
                'title': vm.$t('cansat.notifications.etCansatConnected'), 
                'date': utils.getDate(),
                'code': 0,
                'uuid': utils.generateUUID().toString(),
                'type': vm.$t('cansat.notifications.center.types.info')
            })
        },
        connectFAIL(){
            this.$store.commit('setStatusCanSat', { 'index': 0, 'connected': false})
            this.$store.commit('pushNotificationToast',{ 
                'text': vm.$t('cansat.notifications.etCansatErrorConnect'), 
                'icon': 'fa-check'          
            })
            this.$store.commit('pushNotificationModal',{ 
                'title': vm.$t('cansat.notifications.etCansatErrorConnect'), 
                'date': utils.getDate(),
                'code': 0,
                'uuid': utils.generateUUID().toString(),
                'type': vm.$t('cansat.notifications.center.types.info')
            })
        },
        disconnect(){
            this.clearStatusesOnDisconnect()
            CanSatAPI.disconnectToCansat()
        },  
        validateConnection(){
            if(this.canSatName != '' &&  (this.selectedCansat != '' || this.disableIDList)){
                this.valid = true
            }else{
                this.valid = false
            }
        },
        isValid(){
            return this.isCanSatConnect
        },
        isFormPathValid(field){
            return isValidPath(field) && field != ''
        },
        getName(){
            return this.canSatName
        },
        getSignalLevel(){
            return this.signalLevel = 10
        },
        resetActuators(){
            this.$store.commit('setActuators', defaultActuators.actuators[defaultActuators.parachute])
            this.$store.commit('setActuators', defaultActuators.actuators[defaultActuators.balloon])
        },
        resetSensors(){
            let sensors = defaultSensors.getSensors()
            debugger
            for(let s = 0; s < sensors.length; s++){
                this.$store.commit('addNewSensor', sensors[s])     
            }     
        }
    }
}
</script>

