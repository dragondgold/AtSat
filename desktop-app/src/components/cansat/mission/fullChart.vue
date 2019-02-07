<template>
  <div class="form-wizard-page">    
    <div class="row">
      <div class="col-md-12">
        <vuestic-widget :headerText="$t(sensor.type)" class="pt-0">
            <vuestic-tooltip :options="{ 'content': $t('cansat.resources.sensors.tabs.createdByUser.newSensor.back'), 'placement': 'right'  }">
                <div class="btn btn-info btn-with-icon btn-micro rounded-icon mb-2">
                    <div class="btn-with-icon-content" @click="goBack()" >                                   
                        <i  class="fa fa-arrow-circle-o-left"></i>
                    </div>
                </div>
            </vuestic-tooltip> 
            <chart-widget :sensor="sensor" :showAllSamples="showAllSamples"></chart-widget>
        </vuestic-widget>
      </div>
    </div>
  </div>
</template>

<script>
import chartWidget from './Widgets/chartWidget'

export default {
    name: 'full-chart',
    components:{
        chartWidget
    },
    props: {
        sensor: {
            type: Object,
            required: true
        },
        showAllSamples:{
            type: Boolean,
            required: true,
            default: false
        }
    },
  methods:{
        goBack(){
            this.$router.go(-1)
        }
    }
  }

</script>
