<template>
  <div v-if="CanSatConnected" class="status-bar-container">
    <div style="width:5rem;color:white">
      <i class="fa fa-signal" style="color:blue;margin:0.24rem"></i>
      {{ lqi}}
      <i class="fa fa-battery-full" style="color:green;margin:0.24rem"></i>
      {{ battery}}
    </div>
  </div>
</template>

<script>
export default {
  name: "status-bar",
  computed: {
    rssi() {
      return this.$store.getters.axtec.project.cansat[0].rssi;
    },
    lqi() {
      return this.$store.getters.axtec.project.cansat[0].lqi;
    },
    battery() {
      return this.$store.getters.axtec.project.cansat[0].battery;
    },
    CanSatConnected() {
      return this.$store.getters.axtec.project.cansat[0].connected;
    },
    comPort() {
      return this.$store.getters.axtec.project.earthStation.port;
    }
  }
};
</script>

<style lang="scss">
@import "../../../vuestic-theme/vuestic-sass/resources/resources";

.status-bar-container {
  @include flex-center();
}

.status-container {
  border: 0.2rem;
  border-radius: 10%;
  height: 3rem;
  border-color: red;
  width: 60%;
  border-style: solid;
}
</style>
