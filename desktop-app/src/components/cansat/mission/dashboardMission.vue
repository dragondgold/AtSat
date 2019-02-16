<template>
    <div class="form-wizard-page">
        <div class="row">
            <div class="col-md-12">
                <vuestic-widget class="no-h-padding"
                            :headerText="$t('cansat.mission.dashboard.title')">
                    <div v-if="(isProjectCreated && isTestOk) || isImported">
                        <div class="form-wizard-tab-content">
                            <controls-widget> </controls-widget>
                            <div  v-if="isMissionActive || finished" class="row">
                                <div class="col-md-6" v-for=" (sensor,key) in filterSensors()" :key="key">
                                    <chart-widget :sensor="sensor" :showAllSamples="false"></chart-widget>
                                </div>
                                <div class="col-md-12">
                                    <div class="collapse-page__content">
                                        <div class="col-md-12" style="text-align:center">
                                            <h4> {{ $t('cansat.resources.sensors.gps.title')}}</h4>
                                        </div>
                                        <location-sat> </location-sat>
                                    </div>                                  
                                </div>                  
                            </div>                           
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
import controlsWidget from './Widgets/controlsWidget'
import locationSat from 'components/cansat/sat/locationSat'

export default {
  name: 'dashboard-mission',
  data () {
    return {
        sensors: this.filterSensors()
    }
  },
  components:{
      controlsWidget,chartWidget,locationSat
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
    },
    isMissionActive(){
            return this.$store.getters.axtec.project.cansat[0].missionActive
    },
    finished(){
        return this.$store.getters.axtec.project.cansat[0].missionFinish
    }
  },
  methods:{
        filterSensors(){
            let filtered = this.$store.getters.axtec.project.mission.data.sensors.filter(function(n) {
                return (n._type != 'power' && n._type != 'gps')
            });
            return filtered
        },
        goToNewProject(){
            this.$router.push({name:'newProject'})
        },
        goToTest(){
            this.$router.push({name:'testSat'})
        }
    }
  }

</script>
