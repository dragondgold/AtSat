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
                    v-bind:options="USBList"
                    @input="validateConnection"
                    >
            </vuestic-simple-select>
        </div>
        <div class="form-wizard-tab-content" v-if="!enableWizard" >
            <p v-if="isConnected">  {{$t('cansat.link.etConnectedLabel') + USBPort.toUpperCase()}}  </p>
            <p class="pt-3 mb-0" style="text-align:center">
            <button v-if="!isConnected" class="btn btn-success btn-micro " @click="connect()">
                {{ $t('cansat.link.connect') }}
                <span class="fa fa-link"></span>
            </button>
            <button v-else class="btn btn-danger btn-micro " @click="disconnect()">
                {{ $t('cansat.link.disconnect') }}
                <span class="fa fa-unlink"></span>
            </button>
            </p>
        </div>
    </div>
</template>

<script>
export default {
    name: 'et-widget',
    props: {
        enableWizard: {
            type: Boolean,
            default: true
        }
    },
    created(){
        if(this.$store.getters.axtec.project.earthStation.status == 'Connected'){
            this.isConnected = true
        }
    },
    data () {
        return {
            USBPort: '',
            USBList: [ 'com1', 'com2'],
            valid: false,
            isConnected: false
        }
    },
    computed:{

    },
    methods:{
        connect(){
            this.isConnected = true
            this.$store.commit('setStatusEarthStation', true)
        },
        disconnect(){
            this.isConnected = false
            this.$store.commit('setStatusEarthStation', false)
            this.$store.commit('setStatusCanSat', { 'index': 0, 'connected': false})
        },
        validateConnection(){
            if(this.USBPort != ''){
                this.valid = true
                this.isValid(this.valid)
            }else{
                this.valid = false
                this.isValid(this.valid)
            }
        },
        isValid(){
            return this.valid
        }
    }
}
</script>

