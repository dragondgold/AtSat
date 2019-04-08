<template>
  <vuestic-layout v-layout>
    <app-navbar :isOpen="opened" @toggle-menu="toggleSidebar"/>
    <app-sidebar :isOpen="opened" @toggle-menu="toggleSidebar"/>
    <main slot="content" id="content" class="content" role="main">
      <vuestic-pre-loader v-show="isLoading" class="pre-loader"></vuestic-pre-loader>
      <router-view></router-view>
    </main>
    <span slot="footer">
      &nbsp;
      <a
        href
        @click.prevent="openExternalLink"
        style="font-family:Inversionz; font-size: 2em;"
        class="link"
      >axtec</a>
    </span>
  </vuestic-layout>
</template>

<script>
import VuesticLayout from "../../vuestic-theme/vuestic-components/vuestic-layout/VuesticLayout";
import AppNavbar from "./app-navbar/AppNavbar";
import AppSidebar from "./app-sidebar/AppSidebar";
import Layout from "vuestic-theme/vuestic-directives/Layout";
import { mapGetters } from "vuex";

export default {
  name: "app-layout",

  components: {
    VuesticLayout,
    AppNavbar,
    AppSidebar
  },
  directives: {
    layout: Layout
  },
  data() {
    return {
      opened: false
    };
  },
  methods: {
    toggleSidebar(opened) {
      this.opened = opened;
    },
    openExternalLink() {
      require("electron").shell.openExternal("https://axtec.com.ar/");
    }
  },
  computed: {
    ...mapGetters(["isLoading"])
  }
};
</script>
