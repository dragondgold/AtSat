<template>
  <vuestic-widget class="no-padding no-v-padding" :headerText="this.$t('cansat.link.title')">
    <div class="row">
      <div class="col-md-6 pt-3">
        <et-widget :enableWizard="false"/>
      </div> 
      <div class="col-md-6 pt-3">
        <cansat-widget :enableWizard="false"/>
      </div> 
      <div class="col-md-12"> 
        <p class="pt-3" style="text-align: center">
          <button class="btn btn-success btn-micro " >
            hola
            <span class="glyphicon glyphicon-arrow-right"></span>
          </button>
        </p>
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