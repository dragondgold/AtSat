<template>
  <div class="row">
    <div class="col-md-12 col-sm-12">
      <div class="cards-container" >

        <vuestic-card stripe="info"> 
          
          <template slot="title" >
            <span 
              class="container-icon-home sidebar-menu-item-icon fa fa-file color-icon-home-info">
            </span>
            {{ $t('cards.actionCreateProjectTitle') }}
          </template>
          {{ $t('cards.actionCreateProjectSub') }}
          <p class="pt-3 mb-0">
            <button class="btn btn-info btn-micro float-right" @click="goToNewProject()">
              {{ $t('cards.go') }}
              <span class="glyphicon glyphicon-arrow-right"></span>
            </button>
          </p>          

        </vuestic-card>

        <vuestic-card stripe="success">
          <template slot="title">
            <span 
              class="container-icon-home sidebar-menu-item-icon glyphicon glyphicon-blackboard color-icon-home-sucess">
            </span>
            {{ $t('cards.actionOpenProjectTitle') }}
          </template>
          {{ $t('cards.actionOpenProjectSub') }}
          <p class="pt-3 mb-0">
            <button class="btn btn-success btn-micro float-right" @click="openProject()">
              {{ $t('cards.go') }}
              <span class="glyphicon glyphicon-arrow-right"></span>
            </button>
          </p>

        </vuestic-card>

        <vuestic-card stripe="warning">
          <template slot="title">
            <span 
              class="container-icon-home sidebar-menu-item-icon fa fa-area-chart color-icon-home-warning">
            </span>
            {{ $t('cards.actionOpenMissionTitle') }}
          </template>
          {{ $t('cards.actionOpenMissionSub') }}
          <p class="pt-3 mb-0">
            <button class="btn btn-warning btn-micro float-right" @click="goToOpenMission()">
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
import utils from 'services/utils'

export default {
  name: 'cards',
  components: { VuesticCard },
  data () {
    return {
        isShown: false,
        newProImg: path.resolve('./src/assets/images/cansat.png')
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
      let path =
      dialog.showOpenDialog({ 
          properties: [ 
              'openFile'
          ],        
          filters: [
            {name: 'Custom File Type', extensions: ['cansat_pro']},
          ]

      })
      if(path[0]){
        utils.loadFile(path[0])
      }
      
    },
  
    goToOpenMission() {
      this.$router.push({name:'openProject'})
    }
  },
}
</script>




<style lang="scss">
  $singleGutter: #{(19/16)}rem;

  .color-icon-home-info{
    color: $brand-info
  }

  .color-icon-home-sucess{
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
