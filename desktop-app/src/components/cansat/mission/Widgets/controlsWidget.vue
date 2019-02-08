<template>
    <div class="form-elements" style="text-align:center;padding:1rem;">
        <div class="row">
            <div class="col-md-12">
                <div class="row ">
                    <div class="col-md-12" style="text-align:center"> 
                        <h4>{{ $t('cansat.mission.dashboard.controls.title')}}</h4>
                    </div>
                    <div class="col-md-12" >
                        <table class="table table-striped first-td-padding col-md-12">
                            <thead>
                                <tr>
                                    <td v-if="!isImportedMission">{{$t('cansat.mission.dashboard.controls.projectName')}}</td>
                                    <td>{{$t('cansat.mission.dashboard.controls.missionName')}}</td>
                                    <td >{{$t('cansat.mission.dashboard.controls.cansatName')}}</td>
                                    <td v-if="!isImportedMission">{{$t('cansat.mission.dashboard.controls.statusCansat')}}</td>
                                    <td> {{$t('cansat.mission.dashboard.controls.startDateLabel')}}</td>
                                    <td> {{$t('cansat.mission.dashboard.controls.endDateLabel')}}</td>
                                    <td> {{$t('cansat.mission.dashboard.controls.actions')}}</td>            
                                </tr>
                            </thead>
                            <tbody>
                                <tr>
                                    <td v-if="!isImportedMission">{{getProjectName}}</td>
                                    <td>{{getMissionName}}</td>
                                    <td >{{getCanSatName}}</td>
                                    <td v-if="!isImportedMission">{{getCanSatStatus}}</td>
                                    <td>{{getStartDate}}</td>
                                    <td> {{getEndDate}}</td>
                                    <td> 

                                            <vuestic-tooltip v-if="!isImportedMission && !isMissionActive && !finished" :options="{ 'content': $t('cansat.mission.dashboard.controls.startBtn'), 'placement': 'bottom'  }">
                                                <div class="btn btn-success btn-with-icon btn-micro rounded-icon" style="margin: 0.1rem;">
                                                    <div class="btn-with-icon-content"  @click="startMission()">                                   
                                                        <i  class="fa fa-arrow-circle-right"></i>
                                                    </div>
                                                </div>
                                            </vuestic-tooltip>  

                                            <vuestic-tooltip v-if="!isImportedMission && isMissionActive" :options="{ 'content': $t('cansat.mission.dashboard.controls.saveBtn'), 'placement': 'bottom'  }">
                                                <div class="btn btn-success btn-with-icon btn-micro rounded-icon" style="margin: 0.1rem;">
                                                    <div class="btn-with-icon-content" @click="saveMission()">                                   
                                                        <i  class="fa fa-save"></i>
                                                    </div>
                                                </div>
                                            </vuestic-tooltip> 

                                            <vuestic-tooltip v-if="isMissionActive && !finished" :options="{ 'content': $t('cansat.mission.dashboard.controls.stopBtn'), 'placement': 'bottom'  }">
                                                <div class="btn btn-danger btn-with-icon btn-micro rounded-icon"  style="margin: 0.1rem;">
                                                    <div class="btn-with-icon-content" @click="stopMission()">                                   
                                                        <i  class="fa fa-stop-circle"></i>
                                                    </div>
                                                </div>
                                            </vuestic-tooltip>               

                                            <vuestic-tooltip v-if="finished" :options="{ 'content': $t('cansat.mission.dashboard.controls.exportBtn'), 'placement': 'bottom'  }">
                                                <div class="btn btn-info btn-with-icon btn-micro rounded-icon"  style="margin: 0.1rem;">
                                                    <div class="btn-with-icon-content" @click="exportMission()">                                   
                                                        <i  class="fa fa-download"></i>
                                                    </div>
                                                </div>
                                            </vuestic-tooltip>  
  
                                    </td>
                                </tr>
                            </tbody> 
                        </table>
                        <small v-if="finished">{{$t('cansat.notifications.modal.mission.exportTip')}}</small>
                    </div>
                </div>     
            </div>
        </div>
    </div>
</template>

<script>
var path = require("path")
import utils from 'services/utils'
import MissionManager from 'services/missionManager'

export default {
    name: 'controls-widget',
    data () {
        return {
            
        }
    },
    computed:{
        isImportedMission(){
            return this.$store.getters.axtec.project.cansat[0].missionType == 'imported'
        },
        getProjectName(){
            return path.basename(this.$store.getters.axtec.project.path)
        },
        getMissionName(){
            return path.basename(this.$store.getters.axtec.project.mission.path)
        },
        getCanSatName(){
            if(this.$store.getters.axtec.project.cansat[0].missionType == 'imported'){
                return this.$store.getters.axtec.project.mission.cansatName
            }
            return this.$store.getters.axtec.project.cansat[0].name
        },
        getCanSatStatus(){
            let status = this.$t('cansat.mission.status.problems') 
            if(this.$store.getters.axtec.project.cansat[0].connected && this.$store.getters.axtec.project.cansat[0].testOk){
                status = this.$t('cansat.mission.status.ready') 
            }
            return status
        },
        getStartDate(){
            return this.$store.getters.axtec.project.mission.startDate
        },
        getEndDate(){
            return this.$store.getters.axtec.project.mission.endDate
        },
        isMissionActive(){
            return this.$store.getters.axtec.project.cansat[0].missionActive
        },
        canExport(){
            return this.$store.getters.axtec.project.mission.endDate != ''
        },
        finished(){
            return this.$store.getters.axtec.project.cansat[0].missionFinish
        }
    },
    methods: {
        startMission(){
            this.$store.commit('setMissionStatus',{ 
                cansatIndex: 0,
                missionActive: true
            })
            this.$store.commit('setDateMission',{
                startDate: utils.getDate()
            })
        },
        saveMission(){
            MissionManager.saveMission()
        },
        stopMission(){
            MissionManager.saveMission()
            this.$store.commit('setMissionStatus',{ 
                cansatIndex: 0,
                missionActive: false
            })
            this.$store.commit('setDateMission',{
                endDate: utils.getDate()
            })
            this.$store.commit('setFinishMission',{
                finish:  true
            })
        },
        exportMission(){
            MissionManager.exportToCSV()
        }
    }
}
</script>

<style lang="scss">

</style>