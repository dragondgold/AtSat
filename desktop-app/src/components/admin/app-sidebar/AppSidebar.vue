<template>
  <vuestic-sidebar :hidden="isOpen">
    <template slot="menu">
      <sidebar-link
        :to="{ name: 'home' }">
        <span slot="title">
          <span
            class="sidebar-menu-item-icon vuestic-icon vuestic-icon-dashboard"></span>
          <span>{{ $t('menu.home') }}</span>
        </span>
      </sidebar-link>
      <sidebar-link-group>
        <span slot="title">
          <span
            class="sidebar-menu-item-icon vuestic-icon vuestic-icon-forms"></span>
          <span>{{ $t('menu.project') }}</span>
        </span>
        <sidebar-link
          :to="{ name: 'newProject' }">
          <span slot="title">
            <span>{{ $t('menu.projectNew') }}</span>
          </span>
        </sidebar-link>
        <sidebar-link :to="{ name: 'not-found-simple'}" :event="''" @click.native.prevent="openProject()" >
          <span slot="title"> 
            <span>{{ $t('menu.projectOpen') }}</span>
          </span>
        </sidebar-link>
        <sidebar-link
          :to="{ name: 'saveProject' }">
          <span slot="title">
            <span>{{ $t('menu.projectSave') }}</span>
          </span>
        </sidebar-link>
        <sidebar-link
          :to="{ name: 'configProject' }">
          <span slot="title">
            <span>{{ $t('menu.projectConfig') }}</span>
          </span>
        </sidebar-link>
      </sidebar-link-group>
      <sidebar-link-group>
        <span slot="title">
          <span
            class="sidebar-menu-item-icon vuestic-icon vuestic-icon-maps"></span>
          <span>{{ $t('menu.sat') }}</span>
        </span>
        <sidebar-link
          :to="{ name: 'configSat' }">
          <span slot="title">
            <span>{{ $t('menu.satEnlace') }}</span>
          </span>
        </sidebar-link>
        <sidebar-link
          :to="{ name: 'configSat' }">
          <span slot="title">
            <span>{{ $t('menu.satFuses') }}</span>
          </span>
        </sidebar-link>
        <sidebar-link
          :to="{ name: 'configSat' }">
          <span slot="title">
            <span>{{ $t('menu.satTest') }}</span>
          </span>
        </sidebar-link>
        <sidebar-link
          :to="{ name: 'configSat' }">
          <span slot="title">
            <span>{{ $t('menu.satSensores') }}</span>
          </span>
        </sidebar-link>     
      </sidebar-link-group>
      <sidebar-link-group>
        <span slot="title">
          <span
            class="sidebar-menu-item-icon vuestic-icon vuestic-icon-statistics"></span>
          <span>{{ $t('menu.mission') }}</span>
        </span>
        <sidebar-link
          :to="{ name: 'newMission' }">
          <span slot="title">
            <span>{{ $t('menu.missionNew') }}</span>
          </span>
        </sidebar-link>
        <sidebar-link
          :to="{ name: 'openMission' }">
          <span slot="title">
            <span>{{ $t('menu.missionOpen') }}</span>
          </span>
        </sidebar-link>
      </sidebar-link-group>
    </template>
  </vuestic-sidebar>
</template>

<script>

import VuesticSidebar
  from '../../../vuestic-theme/vuestic-components/vuestic-sidebar/VuesticSidebar'
import SidebarLink from './components/SidebarLink'
import SidebarLinkGroup from './components/SidebarLinkGroup'
import utils from 'services/utils'

const {dialog} = require('electron').remote

export default {
  name: 'app-sidebar',
  methods:{
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
      if(path != null){
        utils.loadFile(path[0])
      }
      
    }
  },
  components: {
    VuesticSidebar,
    SidebarLink,
    SidebarLinkGroup
  },
  props: {
    isOpen: {
      type: Boolean,
      required: true
    }
  }
}

</script>
