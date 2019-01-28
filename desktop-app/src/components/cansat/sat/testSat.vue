<template>
  <div class="form-wizard-page">
    <div class="row">
      <div class="col-md-12">
        <vuestic-widget headerText="Sensores">
          <h1> Integridad del satelite
          </h1>
          <h4> Puede ser un boton para correr test y una consola para ver el estado</h4>
          <h4> Se van a pedir los disintos valores de sensores a la etación terrena con botones </h4>
          <h4> abrir servo paracaidas servo globo </h4>
        </vuestic-widget>
      </div>
    </div>
  </div>
</template>

<script>

export default {
  name: 'test-sat',
  components: {

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
    },
    isETConnected(changes){
      this.canSatConnected = changes
    }
  },

  methods:{

  } 
}
</script>