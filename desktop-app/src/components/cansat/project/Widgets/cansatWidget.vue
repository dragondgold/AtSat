<template>
    <div class="form-wizard-tab-content">
        <div v-if="(ETConnected)">
            <h4 v-if="enableWizard">{{'cansat.project.new.wizard.stepThree.description' | translate}}</h4>
            <h4 v-else>{{'cansat.link.tabs.cansat' | translate}}</h4>
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
            <div class="form-wizard-tab-content" v-if="!enableWizard">
                <p v-if="isCanSatConnected">  {{$t('cansat.link.etConnectedLabel')}}</p>
                <p class="pt-3 mb-0" style="text-align:center">
                    <button v-if="!isCanSatConnected" class="btn btn-success btn-micro" @click="connect()">
                        {{'cansat.link.connect' | translate }}
                        <span class="fa fa-link"></span>
                    </button>
                    <button v-else class="btn btn-danger btn-micro" @click="disconnect()">
                        {{ $t('cansat.link.disconnect') }}
                        <span class="fa fa-unlink"></span>
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
            canSatName: '',
            canSatList: ['id_mcp2210'],
            selectedCansat: '',
            valid: false,
            isCanSatConnected: false
        }
    },
    
    computed: {
        
    },

    watch:{

    },

    computed:{
        ETConnected(){
            return this.$store.getters.axtec.project.earthStation.connected
        },
        CanSatConnected(){
            return this.$store.getters.axtec.project.cansat[0].connected
        }
    },
    methods:{
        connect(){
            this.isCanSatConnected = true
            this.$store.commit('setStatusCanSat', { 'index': 0, 'connected': true})
        },
        disconnect(){
            this.isCanSatConnected = false
            this.$store.commit('setStatusCanSat', { 'index': 0, 'connected': false})
        },
        
        validateConnection(){
            if(this.canSatName != '' && this.selectedCansat != ''){
                this.valid = true
                this.isValid(this.valid)
            }else{
                this.valid = false
                this.isValid(this.valid)
            }
        },
        isValid(){
            return this.valid 
        },
        isFormPathValid(field){
            return isValidPath(field) && field != ''
        }
    }
}
</script>

