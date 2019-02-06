<template>
    <div class="form-wizard-page">
        <div class="row">
            <div class="col-md-12">
                <vuestic-widget class="no-h-padding"
                            :headerText="$t('cansat.mission.new.title')">
                    <div v-if="isProjectCreated && isTestOk" class="form-wizard-tab-content" style="padding: 0.85rem 15%;text-align:center">
                        <p>{{'cansat.mission.new.description' | translate}}</p>
                        <div class="form-group with-icon-right col-md-6" style="margin:0 auto;"
                                :class="{'has-error': !isFormPathValid(missionName), 'valid': isFormPathValid(missionName)}">
                            <div class="input-group">
                                <input
                                name="missionName"
                                data-vv-as="missionName"
                                v-model="missionName"
                                v-validate="'required'"
                                required title=""/>
                                <i
                                class="fa fa-exclamation-triangle error-icon icon-right input-icon"></i>
                                <i class="fa fa-check valid-icon icon-right input-icon"></i>
                                <label class="control-label">
                                {{$t('cansat.project.new.wizard.stepOne.name')}}
                                </label>
                                <i class="bar"></i>
                                <small v-show="!isFormPathValid(missionName)" class="help text-danger">
                                {{$t('cansat.project.new.wizard.stepOne.invalidName')}}
                                </small>
                            </div>
                        </div>
                        <small> {{getPath()}}</small>
                        <p class="pt-3 mb-0">
                            <button v-if="isFormPathValid(missionName)" class="btn btn-micro btn-success" @click="createMission()">
                                {{ $t('cansat.mission.new.create') }}
                            <span class="glyphicon glyphicon-arrow-right"></span>
                            </button>
                        </p> 
                    </div>
                    <div v-else class="row">
                        <div v-if="!isProjectCreated" class="col-md-12" style="text-align:center;">
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
const isValidPath = require('is-valid-path')
import utils from 'services/utils'
import projectManager from 'services/projectManager'
import missionManager from 'services/missionManager'
import chartWidget from './Widgets/chartWidget'
import projectWidget from '../project/Widgets/projectWidget'
import { debug } from 'util';


const {dialog} = require('electron').remote


export default {
  name: 'new-mission',
  data () {
    return {
        debug: this.$store.getters.axtec.debug,
        missionName: '',
        projectPath: this.$store.getters.axtec.project.path,
    }
  },
  computed: {
    isProjectCreated(){
      return this.$store.getters.axtec.project.path != ''        
    },
    isTestOk(){
      return this.$store.getters.axtec.project.cansat[0].testOk
    },
  },
  methods:{
        getPath(){
            if(this.projectPath == '' || this.missionName == ''){
                return ''
            }
            let path = this.projectPath.replace(/\\/g, "/")
            path = path.substring(0, path.lastIndexOf("/")) + '/' + this.missionName + '.cansat_data'
            return path
        },
        isFormPathValid(field){
            if(this.debug && this.projectPath == '' ){
                this.projectPath = 'C:/Users/Leonardo/Desktop/cansat/asd/asd.cansat_pro'
            }
            return isValidPath(field) && field != '' && projectManager.canCreateFile(this.getPath())
        },
        createMission(){       
            this.$store.commit('setPathMission',{ 
                path: this.getPath()
            })
            this.$store.commit('setMissionType',{
                cansatIndex:0,
                missionType: 'created'
            })

            let sensors = JSON.parse(JSON.stringify(this.$store.getters.axtec.project.cansat[0].sensors))
            this.$store.commit('createSensorMission',{
                clear: true
            })
            for(let s= 0; s< sensors.length; s++){
                let samples = []
                if(sensors[s]._type == 'vector'){
                    samples.x = 0
                    samples.y = 0
                    samples.z = 0
                }else{ // It's 'scalar' or 'power' or 'user'
                    samples.lastValue = sensors[s].minValue
                }
                samples.timespan = utils.getDate()

                this.$store.commit('createSensorMission',{
                    id: sensors[s].id,
                    _type: sensors[s]._type
                })

                this.$store.commit('addSensorSample',{
                    index: s,
                    timespan: utils.getDate(),
                    samples: samples
                })
            }
            this.$store.commit('addLocationMission',{
                clear: true
            })
            let lat = this.$store.getters.axtec.project.cansat[0].location.lat
            let lng = this.$store.getters.axtec.project.cansat[0].location.lng
            this.$store.commit('addLocationMission',
                { 
                    lat: lat,
                    lng: lng,
                    timespan: utils.getDate()
                }
            )

            missionManager.saveMission()
            this.$store.commit('pushNotificationToast',{ 
                'text': this.$t('cansat.notifications.modal.mission.createdOk'), 
                'icon': 'fa-check'          
                })
            this.$store.commit('pushNotificationModal',{ 
                'title': this.$t('cansat.notifications.modal.mission.createdOk'), 
                'date': utils.getDate(),
                'code': 0,
                'uuid': utils.generateUUID().toString(),
                'type': this.$t('cansat.notifications.center.types.info')
            })
            this.$router.push({name:'dashboardMission'})
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
