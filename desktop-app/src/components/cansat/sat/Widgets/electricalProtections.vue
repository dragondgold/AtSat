<template>
    <div class="form-wizard-tab-content">
        <table v-if="isConnected" class="table table-striped first-td-padding col-md-12">
            <thead>
                <tr>
                    <td v-for="(field,key) in fields" :key="key">{{$t(field.title)}}</td>             
                </tr>
            </thead>
            <tbody>
                <tr v-for="(suply,key) in powerSuplies" :key="key">
                    <td>{{$t(suply.name)}}</td>
                    <td>{{suply.voltage}}</td>
                    <td>{{suply.current}}</td>
                    <td>{{suply.maxCurrent}}</td>
                    <td> 
                        <i class="fa icon-table" :class="[suply.status =='Active' ? 'fa-check icon-table-success' : 'fa-exclamation-triangle icon-table-danger']" ></i> 
                    </td>
                    <td> 
                        <vuestic-tooltip :options="{ 'content': $t('cansat.protections.table.restart'), 'placement': 'top'  }">
                            <div v-if="suply.status !='Active'" class="btn btn-info btn-with-icon btn-micro rounded-icon">
                                <div class="btn-with-icon-content" @click="resetProtections(key)">                                   
                                    <i  class="fa fa-refresh"></i>
                                </div>
                            </div>
                        </vuestic-tooltip>   
                    </td>
                </tr>
            </tbody> 
        </table>
        <p v-if="powerGood" class="pt-1 mb-3" style="text-align:center">
            <button class="btn btn-success btn-micro" @click="goToSensors()">
                {{'cansat.protections.goToSensors' | translate }}
            </button>
        </p>
        <div v-if="!isConnected">
            <p>{{$t('cansat.protections.noConnected')}}</p>
            <p class="pt-1 mb-3" style="text-align:center">
                <button class="btn btn-success btn-micro" @click="goToLink()">
                    {{'cansat.protections.goToLink' | translate }}
                    <span class="fa fa-link"></span>
                </button>
            </p>
        </div>
    </div>
</template>

<script>
import defaultProtections from 'data/Protections'

export default {
    name: 'electrical-protections',
    props: {
        enableWizard: {
            type: Boolean,
            default: true
        }
    },
    data () {
        return {
            fields: [
                { "title" : 'cansat.protections.table.name'},
                { "title" : 'cansat.protections.table.voltage'}, 
                { "title": 'cansat.protections.table.current'}, 
                { "title": 'cansat.protections.table.maxCurrent'}, 
                { "title" : 'cansat.protections.table.status'},
                { "title" : 'cansat.protections.table.actions'}],
            powerSuplies: this.$store.getters.axtec.project.cansat[0].protections.powerSupply,
            isConnected: this.$store.getters.axtec.project.cansat[0].connected,
            powerGood: false
        }
    },
    created(){
         

    },

    computed: {
        isCanSatConnected(){
            return this.$store.getters.axtec.project.cansat[0].connected
        },
        getPowerSuplies(){
            let list= this.$store.getters.axtec.project.cansat[0].protections.powerSupply
            let errors = list.filter(function(element) {
                return element.status != 'Active'
            })

            if(errors.length > 0 || !this.isConnected){
                this.powerGood = false
            }else{
                this.powerGood = true
            }
            return list
        }
    },

    watch:{
        isCanSatConnected(changes){
            this.isConnected = changes
        },
        getPowerSuplies(changes){
            this.powerSuplies = changes
        }
    },

    methods:{
        goToSensors(){
            this.$router.push({name:'sensorSat'})
        },
        goToLink(){
            this.$router.push({name:'linkSat'})
        },
        resetProtections(key){
            this.$store.commit('setElectricalProtectionsPS',
            { 
                'cansatIndex': 0,
                'psIndex': defaultProtections.vBatt,  // V Batt
                'status': "Active"
            })
            this.$store.commit('setElectricalProtectionsPS',
            { 
                'cansatIndex': 0, 
                'psIndex': defaultProtections.v3v3,   // 3.3 V
                'status' : 'Active'
            })
            this.$store.commit('setElectricalProtectionsPS',
            { 
                'cansatIndex': 0, 
                'psIndex':  defaultProtections.v5v,   // 5 V
                'status' : 'Active'
            })
        }
    }
}
</script>

<style lang="scss">
    .icon-table{
        border-radius: 50%;       
        color: white;
        padding: 0.35rem;
    }
    .icon-table-success {
        background: $brand-primary;
    }
    .icon-table-danger {
        background: $brand-danger;
    }
</style>