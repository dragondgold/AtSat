<template>
  <vuestic-widget class="no-padding no-v-padding" :headerText="this.$t('cansat.link.title')">
    <div v-if="isProjectCreated">
      <div class="row" >
        <div class="col-md-5 pt-3">
          <et-widget :enableWizard="false"/>
        </div> 
        <div class="col-md-7 pt-3">
          <cansat-widget :enableWizard="false"/>
        </div> 
      </div>
      <div v-if="etConnected && canSatConnected" class="col-md-12 pt-1" style="text-align:center">
        <div class="col-md-12">
          <button class="btn btn-success btn-micro mb-3" @click="goToTest()">
              {{'cansat.link.goToTest' | translate }}
              <span class="fa fa-link"></span>
          </button> 
        </div>
          
      </div> 
    </div>
    <div v-else class="row">
      <div class="col-md-12" style="text-align:center;">
        <p>{{$t('cansat.link.projectNoCreated')}}</p>
        <div class="pt-1 mb-3" style="text-align:center">
          <button class="btn btn-success btn-micro" @click="goToNewProject()">
              {{'cansat.project.new.wizard.title' | translate }}
          </button>
        </div>
      </div> 
    </div>
    
  </vuestic-widget>
</template>

<script>
import etWidget from './../project/Widgets/etWidget'
import cansatWidget from './../project/Widgets/cansatWidget'

export default {
  name: 'link-sat',
  components: {
    etWidget,
    cansatWidget
  },
  data () {
    return {
      etConnected: this.$store.getters.axtec.project.cansat[0].connected,
      canSatConnected: this.$store.getters.axtec.project.earthStation.connected,
   }
  },

  created(){

  },

  computed: {
    isCanSatConnected(){
      return this.$store.getters.axtec.project.cansat[0].connected
    },
    isETConnected(){
      return this.$store.getters.axtec.project.earthStation.connected
    },
    isProjectCreated(){
      return this.$store.getters.axtec.project.path != ''        
    }
  },

  watch:{
    isCanSatConnected(changes){
      this.etConnected = changes
    },
    isETConnected(changes){
      this.canSatConnected = changes
    }
  },

  methods:{
    goToTest(){
      this.$router.push({name:'testSat'})
    },
    goToNewProject(){
      this.$router.push({name:'newProject'})
    }
  } 
}
</script>

<style lang="scss">
  .widget-min-height{
    min-height: 25rem;
  }
</style>