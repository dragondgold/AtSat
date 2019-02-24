<template>
    <div class="form-wizard-tab-content">
        <h4 v-if="enableWizard">{{'cansat.project.new.wizard.stepTwo.description' | translate}}</h4>
        <h4 v-else>{{'cansat.link.tabs.et' | translate}}</h4>
        <div v-if="!isConnected">
            
            <p>{{ $t('cansat.project.new.wizard.stepTwo.titlePort') }}</p> 
                    <vuestic-simple-select
                    :label="'cansat.project.new.wizard.stepTwo.selectPort' | translate"
                    v-model="USBPort"
                    name="USBPort"
                    :required="true"
                    ref="USBPort"
                    v-bind:options="serialPorts"
                    @input="validateConnection"
                    >
            </vuestic-simple-select>
        </div>
        <div class="col-md-12" v-if="!enableWizard" >
            <div>
                <p v-if="isConnected">  {{$t('cansat.link.etConnectedLabel') + USBPort.toUpperCase()}}  </p>
                <div class="col-md-12 pt-1 mb-3" style="text-align:center">
                    <button v-if="!isConnected" class="btn btn-success btn-micro " @click="connect()">
                        {{ $t('cansat.link.connect') }}
                        <span class="fa fa-link"></span>
                    </button>
                    <button v-else class="btn btn-danger btn-micro " @click="disconnect()">
                        {{ $t('cansat.link.disconnect') }}
                        <span class="fa fa-unlink"></span>
                    </button>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import CanSatAPI from 'services/CanSatAPI'
import { debug } from 'util';
export default {
    name: 'et-widget',
    props: {
        enableWizard: {
            type: Boolean,
            default: true
        }
    },
    data () {
        return {
            USBPort: '',
            valid: false,
            isConnected: this.$store.getters.axtec.project.earthStation.connected
        }
    },
    created(){
        if(this.enableWizard){
            this.clearStatusesOnDisconnect()
        }
        
    },
    computed:{
        serialPorts(){
            return this.$store.getters.axtec.serialPorts
        }
    },
    watch:{
        serialPorts(ports){
            if(ports.length == 0 && !this.isConnected){
                this.USBPort = ''
            }
        }
    },
    methods:{
        setStatusesOnConnect(){
            this.$store.commit('setPortEarthStation', this.USBPort )
            this.$store.commit('setStatusEarthStation', true )
        },
        clearStatusesOnDisconnect(){
            this.$store.commit('setPortEarthStation', '')
            this.$store.commit('setStatusEarthStation', false)
            this.$store.commit('setStatusCanSat', { 'index': 0, 'connected': false})
            this.$store.commit('setIDCanSat', { 'index': 0, 'id': ''})
            this.$store.commit('setNameCanSat', { 'index': 0, 'name': ''})
            this.isConnected =false
        },
        connect(){
            if(this.USBPort != ''){
                CanSatAPI.connectToPort(this.USBPort)
                this.setStatusesOnConnect()
                this.isConnected = this.$store.getters.axtec.project.earthStation.connected 
            }          
        },
        disconnect(){
            this.clearStatusesOnDisconnect()
            this.isConnected = this.$store.getters.axtec.project.earthStation.connected
            CanSatAPI.disconnectToPort()
        },
        validateConnection(){
            if(this.USBPort != ''){
                this.valid = true
                if(this.enableWizard){
                    this.setStatusesOnConnect()
                }
            }else{
                this.valid = false
            }
        },
        isValid(){
            return this.valid
        }
    }
}
</script>

