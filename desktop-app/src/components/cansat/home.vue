<template>
  <div class="row">
    <div class="col-md-12 col-sm-12">
      <div class="cards-container" >
        <vuestic-card v-for="(card,key) in cards.data" :key="key" :stripe="card.stripe">            
            <template slot="title" >
                <span :class="[card.icon, 'color-icon-home-' + card.stripe]"
                class="container-icon-home sidebar-menu-item-icon ">
                </span>
                <h4>  {{$t(card.title) }}</h4>
               
            </template>
            <p v-if="card.subtitle != undefined"> {{card.subtitle}}</p>
            <p class="pt-3 mb-0">
                <button :class="['btn-' + card.stripe]" class="btn btn-micro float-right" @click="card.go()">
                {{ $t('cards.go') }}
                <span class="glyphicon glyphicon-arrow-right"></span>
                </button>
            </p>          
        </vuestic-card>   
      </div>
    </div>
  </div>
</template>

<script>
import ProyectManager from 'services/projectManager'
import defaultActuators from 'data/Actuators'
import defaultSensors from 'data/Sensors'

export default {
  name: 'home-axtec',
  data () {
    return {
        cards: { 'data': [ 
          { 'stripe' : 'info', 'title' :  'cansat.home.cards.newProject', 'icon' : 'fa fa-file', 'go': this.goToNewProject },
          { 'stripe' : 'success', 'title' : 'cansat.home.cards.openProject', 'icon' : 'glyphicon glyphicon-blackboard', 'go': this.openProject },
          { 'stripe' : 'warning', 'title' : 'cansat.home.cards.loadMission', 'icon' : 'fa fa-area-chart', 'go': this.goToOpenMission }
        ]}
    }
  },

  created(){
    if(this.$store.getters.axtec.debug){ // Initialize data structure
      console.log("Setting up modules", "Debug: " + this.$store.getters.axtec.debug )   
      this.$store.commit('setStatusCanSat', { 'index': 0, 'connected': true})

      // Load actuators from data/Actuators
      this.$store.commit('setActuators', defaultActuators.actuators[defaultActuators.parachute])
      this.$store.commit('setActuators', defaultActuators.actuators[defaultActuators.balloon])

      // Load sensors from data/Sensors
      for(let s = 0; s < defaultSensors.sensors.length; s++){ 
        this.$store.commit('addNewSensor', defaultSensors.sensors[s])
      } 
    }
  },

  methods: {
    goToNewProject() {
      this.$router.push({name:'newProject'})
    },
    openProject() {
      ProyectManager.openProject()
    },
    goToOpenMission() {
      this.$router.push({name:'openProject'})
    }
  },
}
</script>

<style lang="scss">
  .color-icon-home-info{
    color: $brand-info
  }

  .color-icon-home-success{
    color: $brand-success
  }

  .color-icon-home-warning{
    color: $brand-warning
  }

  .container-icon-home{
    display: block;
    font-size: 7rem;
    text-align: center;
    margin-bottom: 0.75rem;
  }
</style>
