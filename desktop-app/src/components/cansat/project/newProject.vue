<template>
  <div class="form-wizard-page">
    <div class="row">
      <div class="col-md-12">
                <vuestic-widget class="no-h-padding widget-min-height"
                        :headerText="$t('cansat.project.new.wizard.title')">
          <vuestic-wizard @wizardComplete="successProject"
            :steps="vsSteps">
            <div slot="page1" class="form-wizard-tab-content">
              <project-widget ref="projectWidget" />
            </div>

            <div slot="page2" class="form-wizard-tab-content">
              <et-widget ref="etWidget"/>
            </div>

            <div slot="page3" class="form-wizard-tab-content">
              <cansat-widget ref="cansatWidget"/>
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
import projectManager from 'services/projectManager'
import { mapGetters } from 'vuex'
import projectWidget from './Widgets/projectWidget'
import etWidget from './Widgets/etWidget'
import cansatWidget from './Widgets/cansatWidget'
import defaultActuators from 'data/Actuators'

const {dialog} = require('electron').remote
const storage = require('electron-json-storage')
const {app} = require('electron')

var that;
export default {
  name: 'cansat-project-wizard',
  components: {
    projectWidget,
    etWidget,
    cansatWidget
  },
  created(){
    //mcp2210.getConnectedDevCount()
  },
  mounted(){
    this.clearStatuses() 
    that = this;
    that.$refs.etWidget.disconnect()
  },
  computed: {
    vsSteps () {
      return [
        {
          label: this.$t('cansat.project.new.wizard.stepOne.label'),
          slot: 'page1',
          onNext: () => {
    
          },
          isValid: () => {
            return that.$refs.projectWidget.isValid()
          },
        },
        {
          label: this.$t('cansat.project.new.wizard.stepTwo.label'),
          slot: 'page2',
          onNext: () => {
      
          },
          isValid: () => {
            return  that.$refs.etWidget.isETConnected()
          },
        },
        {
          label: this.$t('cansat.project.new.wizard.stepThree.label'),
          slot: 'page3',
          onNext: () => {
            
          },
          isValid: () => {
            return  this.$refs.cansatWidget.isValid()
            return  that.$refs.cansatWidget.isValid()
          },
        },
      ]
    },
  },
  data () {
    return {
      
    }
  },
  methods: {
    clearStatuses(){
      console.log("Clear statuses")
      this.$refs.etWidget.clearStatusesOnDisconnect()
      this.$refs.cansatWidget.clearStatusesOnDisconnect()
      this.$store.commit('axtecPath','')
      this.$store.commit('setPathMission', {path:''})
      this.$store.commit('setMissionType',{
          cansatIndex: 0,
          missionType: ''
      })
      this.resetActuators()
    },
    successProject(){
      this.$store.commit('axtecPath', this.$refs.projectWidget.getPath())
      this.$store.commit('setNameCanSat', { 'index': 0, 'name': this.$refs.cansatWidget.getName()})
      this.$refs.etWidget.setStatusesOnConnect()
      this.$refs.cansatWidget.setStatusesOnConnect()
      this.$store.commit('setTestStatus',{ 
        cansatIndex: 0, 
        testOk: false
      })
      this.$store.commit('pushNotificationToast',{ 
          'text': this.$t('cansat.notifications.modal.project.createdOk'), 
          'icon': 'fa-check'          
        })
      this.$store.commit('pushNotificationModal',{ 
          'title': this.$t('cansat.notifications.modal.project.createdOk'), 
          'date': utils.getDate(),
          'code': 0,
          'uuid': utils.generateUUID().toString(),
          'type': this.$t('cansat.notifications.center.types.info')
      })
      projectManager.saveProject()
    },
    goToTestCanSat(){
      this.$router.push({ name: 'testSat' })
    },
    resetActuators(){
      this.$store.commit('setActuators', defaultActuators.actuators[defaultActuators.parachute])
      this.$store.commit('setActuators', defaultActuators.actuators[defaultActuators.balloon])
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
