<template>
  <div class="extra">  
      <vuestic-tabs class="tabs" :names="[$t('cansat.resources.sensors.tabs.location.current'), $t('cansat.resources.sensors.tabs.location.historic')]">
        <div :slot="'cansat.resources.sensors.tabs.location.current' | translate" class="maps-tab">
          <locations-widget></locations-widget>
        </div>
        <div :slot="'cansat.resources.sensors.tabs.location.historic' | translate" class="maps-tab">
          <historic-location></historic-location>
        </div>
      </vuestic-tabs>
      <p v-if="debug" class="pt-1 mb-3" style="text-align:center">
          <button class="btn btn-success btn-micro" @click="newCoor()">
                Test new loc
              <span class="fa fa-link"></span>
          </button>
      </p>
  </div>
</template>

<script>
import locationsWidget from './Widgets/locationsWidget.vue'
import historicLocation from './Widgets/elements/historicLocation.vue'
import utils from 'services/utils'

export default {
  name: 'location-sat',
  components: {
    locationsWidget,
    historicLocation
  },
  data () {
    return {
      debug: this.$store.getters.axtec.debug
    }
  },
  methods:{
    newCoor(){
      this.$store.commit('addNewLocation',{ 
          timespan: utils.getDate(),
          lat: Math.random()*10,
          lng: Math.random()*10  
        })
    }
  }
}
</script>

<style lang="scss">
  .tabs {
    .overview-tab {
      .explore-row {
        display: none !important;
      }
    }

    .maps-tab {
      height: 500px;
    }
  }

  .profile-card-widget, .chat-widget {
    width: 100%;
    .widget-body {
      display: flex;
      justify-content: center;
      align-items: center;
      & > div {
        width: 100%;
      }
    }
  }

  .bottom-widgets {
    > div[class^='col'] {
      & > div {
        width: 100%;
      }
    }
  }
</style>
