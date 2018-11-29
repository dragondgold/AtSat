<template>
  <div class="row">
    <div class="col-md-12 col-sm-12">
      <div class="cards-container" >

        <vuestic-card stripe="info" image="../../assets/images/cansat.png"> 
          <template slot="title" >
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

        <vuestic-card stripe="success" :to="{ name: 'openProject' }">
          <template slot="title">
            {{ $t('cards.actionOpenProjectTitle') }}
          </template>
          {{ $t('cards.actionOpenProjectSub') }}
          <p class="pt-3 mb-0">
            <button class="btn btn-success btn-micro float-right" @click="goToOpenProject()">
              {{ $t('cards.go') }}
              <span class="glyphicon glyphicon-arrow-right"></span>
            </button>
          </p>

        </vuestic-card>

        <vuestic-card stripe="warning" :to="{ name: 'openMission' }">
          <template slot="title">
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
import VuesticCard from '../../vuestic-theme/vuestic-components/vuestic-card/VuesticCard'

export default {
  name: 'cards',
  components: { VuesticCard },
  data () {
    return {
        isShown: false
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
    goToOpenProject() {
      this.$router.push({name:'openProject'})
    },
    goToOpenMission() {
      this.$router.push({name:'openProject'})
    }
  },
}
</script>




<style lang="scss">
  $singleGutter: #{(19/16)}rem;

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
