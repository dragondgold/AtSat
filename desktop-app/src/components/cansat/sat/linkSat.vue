<template>
  <vuestic-widget class="no-padding no-v-padding widget-min-height" :headerText="this.$t('cansat.link.title')">
    <div class="row" >
      <div class="col-md-5 pt-3">
        <et-widget :enableWizard="false"/>
      </div> 
      <div class="col-md-7 pt-3">
        <cansat-widget :enableWizard="false"/>
      </div> 
    </div>
    <p v-if="etConnected && canSatConnected" class="pt-1 mb-3" style="text-align:center">
        <button class="btn btn-success btn-micro" @click="goToTest()">
            {{'cansat.link.goToTest' | translate }}
            <span class="fa fa-link"></span>
        </button>
    </p> 
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
    }
  },

  watch:{
    isCanSatConnected(changes){
      this.etConnected = changes
      if(changes){
        
      }
    },
    isETConnected(changes){
      this.canSatConnected = changes
    }
  },

  methods:{
    goToTest(){
      this.$router.push({name:'testSat'})
    }
  } 
}
</script>

<style lang="scss">
  .widget-min-height{
    min-height: 25rem;
  }
</style>