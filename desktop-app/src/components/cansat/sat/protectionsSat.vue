<template>
  <div class="form-wizard-page">
    <div class="row">
      <div class="col-md-12">
        <vuestic-widget :headerText="$t('cansat.protections.title')" class="pt-0">
          <electrical-protections/>
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