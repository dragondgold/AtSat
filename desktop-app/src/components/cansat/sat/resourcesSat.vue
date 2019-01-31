<template>
  <div class="form-wizard-page">
    <div class="row">
      <div class="col-md-12">
        <vuestic-widget :headerText="$t('cansat.resources.title')" class="pt-0">
          <vuestic-accordion v-if="isConnected">
            <vuestic-collapse class="collapse-background" ref="collapse">
              <span slot="header"> {{$t('cansat.resources.actuators.title')}} </span>
              <div slot="body">
                <div class="row">
                  <div class="col-md-12">
                      <div class="collapse-page__content">
                        <actuators-widget/>
                      </div>
                  </div>
                </div>
              </div>
            </vuestic-collapse>
            <vuestic-collapse>
              <span slot="header"> {{$t('cansat.resources.sensors.title')}} </span>
              <div slot="body">
                <div class="row">
                  <div class="col-md-12">
                    <div class="collapse-page__content">
                      <sensors-widget/>
                    </div>
                  </div>
                </div>
              </div>
            </vuestic-collapse>
            <vuestic-collapse>
              <span slot="header"> {{$t('cansat.resources.sensors.tabs.powerSupplies.title')}} </span>
              <div slot="body">
                <div class="row">
                  <div class="col-md-12">
                    <div class="collapse-page__content">
                      <powerSuppliesWidget/>
                    </div>
                  </div>
                </div>
              </div>
            </vuestic-collapse>
            <vuestic-collapse>
              <span slot="header"> {{$t('cansat.resources.sensors.gps.title')}} </span>
              <div slot="body">
                <div class="row">
                  <div class="col-md-12">
                    <div class="collapse-page__content">
                      <locationsWidget/>
                    </div>
                  </div>
                </div>
              </div>
            </vuestic-collapse>
          </vuestic-accordion>
          <div v-else>
            <p>{{$t('cansat.protections.noConnected')}}</p>
            <p class="pt-1 mb-3" style="text-align:center">
                <button class="btn btn-success btn-micro" @click="goToLink()">
                    {{'cansat.protections.goToLink' | translate }}
                    <span class="fa fa-link"></span>
                </button>
            </p>
          </div>          
        </vuestic-widget>
      </div>
    </div>
  </div>
</template>

<script>
import actuatorsWidget from './Widgets/actuatorsWidget'
import sensorsWidget from './Widgets/sensorsWidget'
import powerSuppliesWidget from './Widgets/powerSuppliesWidget'
import locationsWidget from './Widgets/locationsWidget'

import defaultActuators from 'data/Actuators'
import defaultSensors from 'data/Sensors'

export default {
    name: 'resources-sat',
    components:{
      actuatorsWidget,
      sensorsWidget,
      powerSuppliesWidget,
      locationsWidget
    },
    data () {
        return {
          isConnected: this.$store.getters.axtec.project.cansat[0].connected,
        }
    },

  created(){

  },

    mounted(){
      if( this.$refs.collapse){
        this.$refs.collapse.expand()
      }
    },

    computed:{
      isCanSatConnected(){
            return this.$store.getters.axtec.project.cansat[0].connected
      },
    },

    watch:{
       isCanSatConnected(changes){
            this.isConnected = changes
        }
    },
    methods:{
      goToLink(){
        this.$router.push({name:'linkSat'})
      }
    }
}
</script>

<style lang="scss">
  .vuestic-collapse__header__button {
    background-color:$darkest-gray;
    color: white;
  }
  .input--textfield {
    border: none;
    color: none;
    background: none;
    display: inline-block;
    text-shadow: 0 0 0 gray;
    width: 3.5rem;

    &:focus {
    outline: none;
    }
  }
  .disable-caret{
    border: none;
    color: transparent;
    text-shadow: 0 0 0 gray;
    text-align: center;

    &:focus {
        outline: none;
    }
  }

</style>