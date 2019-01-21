<template>
  <div class="form-wizard-page">
    <div class="row">
      <div class="col-md-12">
                <vuestic-widget class="no-h-padding"
                        :headerText="$t('cansat.project.new.wizard.tittle')">
          <vuestic-wizard @wizardComplete="successProject"
            :steps="vsSteps">
            <div slot="page1" class="form-wizard-tab-content">
              <h4>{{'cansat.project.new.wizard.stepOne.description' | translate}}</h4>
              <p>{{ $t('cansat.project.new.wizard.stepOne.titleName') }}</p> 
              <div class="form-group with-icon-right"
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

            <div slot="page2" class="form-wizard-tab-content">
              <h4>{{'cansat.project.new.wizard.stepTwo.description' | translate}}</h4>
              <p>{{ $t('cansat.project.new.wizard.stepTwo.titlePort') }}</p>    
              <vuestic-simple-select
                :label="'cansat.project.new.wizard.stepTwo.selectPort' | translate"
                v-model="USBPort"
                name="usbPort"
                :required="true"
                ref="usbPort"
                v-bind:options="USBList">
              </vuestic-simple-select>
            </div>

            <div slot="page3" class="form-wizard-tab-content">
              <h4>{{'cansat.project.new.wizard.stepThree.description' | translate}}</h4>
              <p>{{ $t('cansat.project.new.wizard.stepThree.titleCanSat') }}</p>    
              <vuestic-simple-select
                :label="'cansat.project.new.wizard.stepThree.selectCanSat' | translate"
                v-model="selectedCansat"
                name="selectedCansat"
                :required="true"
                ref="selectedCansat"
                v-bind:options="CanSatList">
              </vuestic-simple-select>
              <p>{{ $t('cansat.project.new.wizard.stepThree.titleName') }}</p>    
              <div class="form-group with-icon-right"
                   :class="{'has-error': !isFormPathValid(cansatName), 'valid': isFormPathValid(cansatName)}">
                <div class="input-group">
                  <input
                    name="cansatName"
                    data-vv-as="Name"
                    v-model="cansatName"
                    v-validate="'required'"
                    required title=""/>
                  <i
                    class="fa fa-exclamation-triangle error-icon icon-right input-icon"></i>
                  <i class="fa fa-check valid-icon icon-right input-icon"></i>
                  <label class="control-label">
                    {{$t('cansat.project.new.wizard.stepOne.name')}}
                  </label>
                  <i class="bar"></i>
                  <small v-show="!isFormPathValid(cansatName)" class="help text-danger">
                    {{$t('cansat.project.new.wizard.stepOne.invalidName')}}
                  </small>
                </div>
              </div>
            </div>
            <div slot="wizardCompleted" class="form-wizard-tab-content">
              <h4>{{'forms.wizard.completed' | translate}}</h4>
              <p> {{'cansat.project.new.wizard.stepLast.description' | translate}}</p>
              <p class="pt-3 mb-0 float-right">
                <button class="btn btn-success btn-micro " @click="goToTestCanSat()">
                  {{ $t('cansat.project.new.wizard.stepLast.goToTest') }}
                  <span class="glyphicon glyphicon-arrow-right"></span>
                </button>
              </p>
            </div>
          </vuestic-wizard>
        </vuestic-widget>
      </div>
    </div>
  </div>
</template>

<script>
import CountriesList from 'data/CountriesList'
import utils from 'services/utils'
import mcp2210 from 'services/mcp2210'
import { mapGetters } from 'vuex';

const {dialog} = require('electron').remote
const storage = require('electron-json-storage')
const {app} = require('electron')
const isValidPath = require('is-valid-path')
const log = require('electron-log')
const homedir = require('os').homedir()

export default {
  name: 'cansat-project-wizard',
  computed: {
    vsSteps () {
      return [
        {
          label: this.$t('cansat.project.new.wizard.stepOne.label'),
          slot: 'page1',
          onNext: () => {
            
          },
          isValid: () => {
            let valid = this.isFormPathValid(this.getPath())
            if(valid){
              
            }
            return valid
          },
        },
        {
          label: this.$t('cansat.project.new.wizard.stepTwo.label'),
          slot: 'page2',
          onNext: () => {
            
          },
          isValid: () => {
            return true
          },
        },
        {
          label: this.$t('cansat.project.new.wizard.stepThree.label'),
          slot: 'page3',
        },
      ]
    },
  },
  data () {
    return {
      projectName: '',
      cansatName: '',
      projectLocation: homedir,
      USBPort: '',
      USBList: [ 'com1', 'com2'],
      CanSatList: ['id_mcp2210'],
      selectedCansat: '',
    }
  },
  methods: {
    successProject(){
      console.log("CREADO OK")
      this.$store.commit('axtecPath',this.getPath())
      this.$store.commit('pushNotificationToast',{ 
          'text': 'Creando nuevo proyecto', 
          'icon': 'fa-commenting'          
        })
    },
    goToTestCanSat(){
      this.$router.push({ name: 'testSat' })
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

<style lang="scss">
  .widget.simple-vertical-wizard-widget {
    .widget-body {
      padding: 0 $widget-padding;
      @include media-breakpoint-down(sm) {
        padding: $widget-padding 0;
      }
    }
  }
</style>
