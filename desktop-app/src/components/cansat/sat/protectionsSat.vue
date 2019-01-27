<template>
  <div class="form-wizard-page">
    <div class="row">
      <div class="col-md-12">
        {{$t('cansat.protections.title')}}
        <vuestic-widget :headerText="$t('cansat.protections.title')" class="pt-0">
          <vuestic-tabs
          :names="[$t('cansat.protections.tabs.electrical.title'), $t('cansat.protections.tabs.physical.title')]"
          ref="tabs">
            <div :slot="$t('cansat.protections.tabs.electrical.title')">
              <electrical-protections/>
            </div>
            <div :slot="$t('cansat.protections.tabs.physical.title')">

            </div>
          </vuestic-tabs>
        </vuestic-widget>
      </div>
    </div>
  </div>
</template>

<script>
import electricalProtections from './Widgets/electricalProtections'

export default {
    name: 'protections-sat',
    components:{
      electricalProtections
    },
    data () {
        return {

        }
    },
    created(){

    },
    methods:{

    }
}
</script>