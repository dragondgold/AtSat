<template>
  <div class="form-wizard-page">
    <div class="row">
      <div class="col-md-12">
                <vuestic-widget class="no-h-padding"
                        :headerText="$t('cansat.project.new.wizard.tittle')">
          <vuestic-wizard
            :steps="vsSteps">
                        <div slot="page1" class="form-wizard-tab-content">
              <div class="form-group with-icon-right"
                   :class="{'has-error': !isFormPathValid(vsName), 'valid': isFormPathValid(vsName)}">
                <div class="input-group">
                  <input
                    name="vsName"
                    data-vv-as="Name"
                    v-model="vsName"
                    v-validate="'required'"
                    required title=""/>
                  <i
                    class="fa fa-exclamation-triangle error-icon icon-right input-icon"></i>
                  <i class="fa fa-check valid-icon icon-right input-icon"></i>
                  <label class="control-label">
                    {{$t('cansat.project.new.wizard.stepOne.name')}}
                  </label>
                  <i class="bar"></i>
                  <small v-show="!isFormPathValid(vsName)" class="help text-danger">
                    {{$t('cansat.project.new.wizard.stepOne.invalidName')}}
                  </small>
                </div>
              </div>
              <div class="form-group form-group-w-btn" :class="{'has-error': !isFormPathValid(vsLocation), 'valid': isFormPathValid(vsLocation)}">
                <div class="input-group">
                  <input id="input-w-btn-round" required  v-validate="'required'" data-vv-as="Location" name="vsLocation" v-model="vsLocation"/>
                  <label class="control-label" for="input-w-btn-round">
                    {{$t('cansat.project.new.wizard.stepOne.location')}}
                  </label>
                  <i class="bar"></i>
                  <small v-show="!isFormPathValid(vsLocation)" class="help text-danger">
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
              
              <div class="form-wizard-tab-content-text" v-show="isFormPathValid(vsName)">
                <p>{{getPath()}}</p>
              </div>        
            </div>
            <div slot="page2" class="form-wizard-tab-content">
              <div class="form-wizard-tab-content-text">
                <p>Zebras communicate with facial expressions and sounds. They
                  make loud braying or barking sounds and
                  soft snorting sounds. The position of their ears, how wide
                  open their eyes are, and whether they show
                  their teeth all send a signal. For example, ears flat back
                  means trouble, or "you better follow orders!"</p>
              </div>

            </div>
            <div slot="page3" class="form-wizard-tab-content">
              <h4>{{'forms.wizard.confirmSelection' | translate}}</h4>
              <p>
                Zebras communicate with facial expressions and sounds. They make
                loud braying or barking sounds and
                soft snorting sounds. The position of their ears, how wide open
                their eyes are, and whether they show
                their teeth all send a signal. For example, ears flat back means
                trouble, or "you better follow orders!"
              </p>
            </div>
            <div slot="wizardCompleted" class="form-wizard-tab-content">
              <h4>{{'forms.wizard.completed' | translate}}</h4>
              <p>
                Zebras communicate with facial expressions and sounds. They make
                loud braying or barking sounds and
                soft snorting sounds. The position of their ears, how wide open
                their eyes are, and whether they show
                their teeth all send a signal. For example, ears flat back means
                trouble, or "you better follow orders!"
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
  name: 'form-wizard',
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
              this.$store.commit('axtecPath',this.getPath())
              this.$store.commit('pushNotificationToast',{ 
                'text': 'Creando nuevo proyecto', 
                'icon': 'fa-commenting'          
              })
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
      vsName: '',
      vsLocation: homedir,
    }
  },
  methods: {
    getPath(){
      if(this.$data.vsLocation == '' || this.$data.vsName == ''){
        return ''
      }
      return this.$data.vsLocation + '\\'+  this.$data.vsName + '\\'+  this.$data.vsName + '.cansat_pro'
    },
    searchFolder(){
      let files = dialog.showOpenDialog({
        properties: ['openDirectory']
      })
      if(files[0] != null){

        this.$data.vsLocation = files[0]
        //let json = JSON.stringify(this.$store.getters.axtec.project)
        //utils.createFile(this.getPath(),json)
      }else{
        this.$data.vsLocation = ''
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
