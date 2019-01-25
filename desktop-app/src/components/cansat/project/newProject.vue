<template>
  <div class="form-wizard-page">
    <div class="row">
      <div class="col-md-12">
                <vuestic-widget class="no-h-padding"
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
import mcp2210 from 'services/mcp2210'
import { mapGetters } from 'vuex'
import projectWidget from './Widgets/projectWidget'
import etWidget from './Widgets/etWidget'
import cansatWidget from './Widgets/cansatWidget'


const {dialog} = require('electron').remote
const storage = require('electron-json-storage')
const {app} = require('electron')

export default {
  name: 'cansat-project-wizard',
  components: {
    etWidget,
    projectWidget,
    cansatWidget
  },
  created(){
    mcp2210.getConnectedDevCount()
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

            return this.$refs.projectWidget.isValid()
          },
        },
        {
          label: this.$t('cansat.project.new.wizard.stepTwo.label'),
          slot: 'page2',
          onNext: () => {
            
          },
          isValid: () => {
            return this.$refs.etWidget.isValid()
          },
        },
        {
          label: this.$t('cansat.project.new.wizard.stepThree.label'),
          slot: 'page3',
          onNext: () => {
            
          },
          isValid: () => {
            return this.$refs.cansatWidget.isValid()
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
    successProject(){
      //this.$store.commit('axtecPath',this.getPath())
      this.$store.commit('pushNotificationToast',{ 
          'text': 'Creando nuevo proyecto', 
          'icon': 'fa-commenting'          
        })
    },
    goToTestCanSat(){
      this.$router.push({ name: 'testSat' })
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
