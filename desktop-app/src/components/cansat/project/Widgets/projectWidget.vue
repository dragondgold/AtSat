<template>
    <div class="form-wizard-tab-content">
        <h4 v-if="showDescription">{{'cansat.project.new.wizard.stepOne.description' | translate}}</h4>
        <p>{{ $t('cansat.project.new.wizard.stepOne.titleName') }}</p> 
        <div class="form-group with-icon-right" style="width: inherit"
            :class="{'has-error': !isFormPathValid(projectName), 'valid': isFormPathValid(projectName)}">
        <div class="input-group">
            <input
            name="projectName"
            data-vv-as="Name"
            v-model="projectName"
            v-validate="'required'"
            required title=""/>
            <i
            class="fa fa-exclamation-triangle error-icon icon-right input-icon"></i>
            <i class="fa fa-check valid-icon icon-right input-icon"></i>
            <label class="control-label">
            {{$t('cansat.project.new.wizard.stepOne.name')}}
            </label>
            <i class="bar"></i>
            <small v-show="!isFormPathValid(projectName)" class="help text-danger">
            {{$t('cansat.project.new.wizard.stepOne.invalidName')}}
            </small>
        </div>
        </div>
        <p>{{ $t('cansat.project.new.wizard.stepOne.titleLocation') }}</p> 
        <div class="form-group form-group-w-btn" :class="{'has-error': !isFormPathValid(projectLocation), 'valid': isFormPathValid(projectLocation)}">
        <div class="input-group">
            <input id="input-w-btn-round" required  v-validate="'required'" data-vv-as="Location" name="projectLocation" v-model="projectLocation"/>
            <label class="control-label" for="input-w-btn-round">
            {{$t('cansat.project.new.wizard.stepOne.location')}}
            </label>
            <i class="bar"></i>
            <small v-show="!isFormPathValid(projectLocation)" class="help text-danger">
            {{$t('cansat.project.new.wizard.stepOne.invalidLocation')}}
            </small>
        </div>
        <div
            class="btn btn-primary btn-with-icon btn-micro rounded-icon" @click.prevent="searchFolder()">
            <div class="btn-with-icon-content">
            <i class="ion-md-folder-open ion"></i>
            </div>
        </div>
        </div>
        
        <div class="form-wizard-tab-content-text" v-show="isFormPathValid(projectName)">
        <p>{{getPath()}}</p>
        </div> 
    </div>
</template>

<script>
const {dialog} = require('electron').remote
const isValidPath = require('is-valid-path')
const homedir = require('os').homedir()

export default {
    name: 'project-widget',
    props: {
        showDescription: {
            type: Boolean,
            default: true
        }
    },
    data () {
        return {
            projectName: '',
            projectLocation: homedir,
        }
    },
    computed:{

    },
    methods:{
        isValid(){
            return this.isFormPathValid(this.getPath())
        },
        getPath(){
            if(this.$data.projectLocation == '' || this.$data.projectName == ''){
                return ''
            }
            return this.$data.projectLocation + '\\'+  this.$data.projectName + '\\'+  this.$data.projectName + '.cansat_pro'
        },
        searchFolder(){
        let files = dialog.showOpenDialog({
            properties: ['openDirectory']
        })
        if(files[0] != null){

            this.$data.projectLocation = files[0]
            //let json = JSON.stringify(this.$store.getters.axtec.project)
            //utils.createFile(this.getPath(),json)
        }else{
            this.$data.projectLocation = ''
        }
        },
        isFormPathValid(field){
            return isValidPath(field) && field != ''
        }

    }
}
</script>

