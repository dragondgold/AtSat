<template>
  <div class="row">
    <div class="col-md-12 col-sm-12">
      <div class="cards-container" >
        <vuestic-card v-for="(card,key) in cards.data" :key="key" :stripe="card.stripe">            
            <template slot="title" >
                <span :class="[card.icon, 'color-icon-home-' + card.stripe]"
                class="container-icon-home sidebar-menu-item-icon ">
                </span>
                <h4>  {{$t(card.title) }}</h4>
               
            </template>
            <p v-if="card.subtitle != undefined"> {{card.subtitle}}</p>
            <p class="pt-3 mb-0">
                <button :class="['btn-' + card.stripe]" class="btn btn-micro float-right" @click="card.go()">
                {{ $t('cards.go') }}
                <span class="glyphicon glyphicon-arrow-right"></span>
                </button>
            </p>          
        </vuestic-card>   
      </div>
    </div>
  </div>
</template>

<script>
const path = require('path')
const {dialog} = require('electron').remote

import VuesticCard from '../../vuestic-theme/vuestic-components/vuestic-card/VuesticCard'
import ProyectManager from 'services/projectManager'

export default {
  name: 'home-axtec',
  components: { VuesticCard },
  data () {
    return {
        isShown: false,
        newProImg: path.resolve('./src/assets/images/cansat.png'),
        cards: { 'data': [ 
          { 'stripe' : 'info', 'title' :  'cansat.home.cards.newProject', 'icon' : 'fa fa-file', 'go': this.goToNewProject },
          { 'stripe' : 'success', 'title' : 'cansat.home.cards.openProject', 'icon' : 'glyphicon glyphicon-blackboard', 'go': this.openProject },
          { 'stripe' : 'warning', 'title' : 'cansat.home.cards.loadMission', 'icon' : 'fa fa-area-chart', 'go': this.goToOpenMission }
        ]}
    }
  },
  methods: {
    addCards () {
      this.isShown = true
      setTimeout(() => {
        this.isShown = false
        ++this.listLoops
      }, 1000)
    },
    goToNewProject() {
      this.$router.push({name:'newProject'})
    },
    openProject() {
      ProyectManager.openProject()
    },
  
    goToOpenMission() {
      this.$router.push({name:'openProject'})
    }
  },
  created(){

  }
}
</script>




<style lang="scss">
  $singleGutter: #{(19/16)}rem;

  .color-icon-home-info{
    color: $brand-info
  }

  .color-icon-home-success{
    color: $brand-success
  }

  .color-icon-home-warning{
    color: $brand-warning
  }

  .container-icon-home{
    display: block;
    font-size: 7rem;
    text-align: center;
    margin-bottom: 0.75rem;
  }

  .cards-container {
    display: flex;
    flex-wrap: wrap;
    margin: -$singleGutter;
    align-items: flex-start;
    .vuestic-card {
      margin: $singleGutter;

      width: calc(33% - #{$singleGutter} * 2);

      @include media-breakpoint-only(xl) {
        width: calc(25% - #{$singleGutter} * 2);
      }

      @include media-breakpoint-only(lg) {
        width: calc(33.3% - #{$singleGutter} * 2);
      }

      @include media-breakpoint-only(sm) {
        width: calc(50% - #{$singleGutter} * 2);
      }

      @include media-breakpoint-only(xs) {
        width: calc(100% - #{$singleGutter} * 2);
      }
    }
  }

  .pre-loader-container {
    height: 50px;
    margin-top: 50px;
    margin-bottom: 50px;
  }
</style>
