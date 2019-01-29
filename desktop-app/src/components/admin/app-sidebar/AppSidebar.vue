<template>
  <vuestic-sidebar :hidden="isOpen">
    <template slot="menu">
      <sidebar-link
        :to="{ name: 'home' }">
        <span slot="title">
          <span
            class="sidebar-menu-item-icon fa fa-home"></span>
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
        <sidebar-link :to="{ name: 'not-found-simple'}" :event="''" @click.native.prevent="openExistingProject()" >
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
      </sidebar-link-group>
      <sidebar-link-group>
        <span slot="title">
          <span
            class="sidebar-menu-item-icon fa-rocket fa"></span>
          <span>{{ $t('menu.sat') }}</span>
        </span>
        <sidebar-link
          :to="{ name: 'linkSat' }">
          <span slot="title">
            <span>{{ $t('menu.satEnlace') }}</span>
          </span>
        </sidebar-link>
        <sidebar-link
          :to="{ name: 'testSat' }">
          <span slot="title">
            <span>{{ $t('menu.satTest') }}</span>
          </span>
        </sidebar-link>
        <sidebar-link
          :to="{ name: 'protectionsSat' }">
          <span slot="title">
            <span>{{ $t('menu.satProtections') }}</span>
          </span>
        </sidebar-link>
        <sidebar-link
          :to="{ name: 'sensorSat' }">
          <span slot="title">
            <span>{{ $t('menu.satSensores') }}</span>
          </span>
        </sidebar-link>     
      </sidebar-link-group>
      <sidebar-link-group>
        <span slot="title">
          <span
            class="sidebar-menu-item-icon fa fa-line-chart"></span>
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
import ProjectManager from 'services/projectManager'

export default {
  name: 'app-sidebar',
  methods:{
    openExistingProject(){
      ProjectManager.openProject()
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
