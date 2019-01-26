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
      
   }
  },

  created(){

  },

  computed: {

  },

  watch:{

  },

  methods:{

  } 
}
</script>

<style lang="scss">
  .widget-min-height{
    min-height: 25rem;
  }
</style>