<template>
    <div class="form-wizard-tab-content">
        <div v-if="ETConnected">
            <h4 v-if="enableWizard">{{'cansat.project.new.wizard.stepThree.description' | translate}}</h4>
            <h4 v-else>{{'cansat.link.tabs.cansat' | translate}}</h4>
            <div v-if="!isCanSatConnect">
                <p>{{ $t('cansat.project.new.wizard.stepThree.titleCanSat') }}</p>    
                <vuestic-simple-select
                :label="'cansat.project.new.wizard.stepThree.selectCanSat' | translate"
                v-model="selectedCansat"
                name="selectedCansat"
                :required="true"
                ref="selectedCansat"
                v-bind:options="canSatList" disabled>
                </vuestic-simple-select>
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
                            <td>{{cansat.id}}</td>
                            <td>{{cansat.name}}</td>
                            <td>{{cansat.signal}}</td>
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

            <div class="form-wizard-tab-content" v-if="!enableWizard">         
                <p class="pt-1 mb-3" style="text-align:center">
                    <button v-if="!isCanSatConnect" class="btn btn-success btn-micro" @click="connect()">
                        {{'cansat.link.connect' | translate }}
                        <span class="fa fa-link"></span>
                    </button>
                </p>
            </div>
        </div>
        <div v-else>
            <h4>{{'cansat.link.tabs.cansat' | translate}}</h4>
            <p>{{'cansat.link.etDisconnectedLabel' | translate}}</p>
        </div>
    </div>
</template>

<script>
const isValidPath = require('is-valid-path')
import defaultActuators from 'data/Actuators'
import defaultSensors from 'data/Sensors'

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
            signalLevel: '0',
            valid: false,
            isCanSatConnect: this.$store.getters.axtec.project.cansat[0].connected,
            fields: [ {'title':  'cansat.link.table.id' },{'title': 'cansat.link.table.name'}, {'title':'cansat.link.table.signal'}, {'title':'cansat.link.table.action'}],
            cansats: [this.$store.getters.axtec.project.cansat[0]]
        }
    },
    
    computed:{
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
            this.resetActuators()
            this.createSensors()          
        },
        clearStatusesOnDisconnect(){
            this.$store.commit('setStatusCanSat', { 'index': 0, 'connected': false})
            this.$store.commit('setIDCanSat', { 'index': 0, 'id': ''})
            this.$store.commit('setNameCanSat', { 'index': 0, 'name': ''})
            this.$store.commit('setSignalCanSat', { 'index': 0, 'signal': ''})
        },
        connect(){
            if( this.canSatName != '' &&  this.selectedCansat != ''){
                this.setStatusesOnConnect()
            }
        },
        disconnect(){
            this.clearStatusesOnDisconnect()
        },  
        validateConnection(){
            if(this.canSatName != '' && this.selectedCansat != ''){
                this.valid = true
            }else{
                this.valid = false
            }
        },
        isValid(){
            return this.valid 
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
        createSensors(){
            for(let s = 0; s < defaultSensors.sensors.length; s++){
                this.$store.commit('addNewSensor', defaultSensors.sensors[s])
            }     
        }
    }
}
</script>

