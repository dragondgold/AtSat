<template>
    <div class="form-wizard-tab-content">
        <h4>{{'cansat.project.new.wizard.stepThree.description' | translate}}</h4>
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
</template>

<script>
const isValidPath = require('is-valid-path')

export default {
    name: 'cansat-widget',
    data () {
        return {
            canSatName: '',
            canSatList: ['id_mcp2210'],
            selectedCansat: '',
            valid: false
        }
    },
    computed:{

    },
    methods:{
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

