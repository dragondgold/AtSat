<template>
    <div class="form-wizard-page">
        <div class="row">
            <div class="col-md-12">
                <vuestic-widget class="no-h-padding widget-min-height"
                            :headerText="$t('cansat.mission.dashBoard.title')">
                    <div v-if="(isProjectCreated && isTestOk) || isImported" class="wizard-body horizontal" style="padding: 0.85rem 15%;">
                        <div class="form-wizard-tab-content">

                        </div>
                    </div>
                    <div v-else>
                        <div v-if="!isProjectCreated" class="col-md-12" style="text-align:center;" >
                            <p>{{$t('cansat.link.projectNoCreated')}}</p>
                            <div class="pt-1 mb-3" style="text-align:center">
                            <button class="btn btn-success btn-micro" @click="goToNewProject()">
                                {{'cansat.project.new.wizard.title' | translate }}
                            </button>
                            </div>
                        </div>
                        <div v-else class="col-md-12" style="text-align:center;">
                            <p>{{$t('cansat.test.noTestOk')}}</p>
                            <div class="pt-1 mb-3" style="text-align:center">
                            <button class="btn btn-success btn-micro" @click="goToTest()">
                                {{'cansat.test.title' | translate }}
                            </button>
                            </div>
                        </div>
                    </div>
                </vuestic-widget>
            </div>
        </div>
    </div>
</template>

<script>
import chartWidget from './Widgets/chartWidget'
import projectWidget from '../project/Widgets/projectWidget'

const {dialog} = require('electron').remote
const isValidPath = require('is-valid-path')
const homedir = require('os').homedir()

export default {
  name: 'dashboard-mission',
  data () {
    return {

    }
  },
  computed: {
    isProjectCreated(){
        return this.$store.getters.axtec.project.path != ''        
    },
    isTestOk(){
        return this.$store.getters.axtec.project.cansat[0].testOk
    },
    isImported(){
        return this.$store.getters.axtec.project.cansat[0].missionType == 'imported'
    }
  },
  methods:{
        goToNewProject(){
            this.$router.push({name:'newProject'})
        },
        goToTest(){
            this.$router.push({name:'testSat'})
        }
    }
  }

</script>
