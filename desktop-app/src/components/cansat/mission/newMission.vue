<template>
  <div>
    <div class="row">
      <div class="col-md-12">
        <vuestic-widget :headerText="$t('cansat.mission.new.title')">
          <div class="col-md-12 pt-3" >
            <h4> {{ $t('cansat.mission.new.description')}} </h4>
          </div>
          <chart-widget></chart-widget>
        </vuestic-widget>
      </div>
    </div>
  </div>
</template>

<script>
import chartWidget from './Widgets/chartWidget'
export default {
  name: 'new-mission',
  components:{
    chartWidget
  },
  data () {
    return {

    }
  }
}
</script>
