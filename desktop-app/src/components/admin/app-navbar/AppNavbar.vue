<template>
  <vuestic-navbar>
    <header-selector slot="selector" :isOpen.sync="valueProxy"/>
    <span slot="logo" class="col nav-item">
      <vuestic-icon-vuestic/>
    </span>
    <span slot="center">
      {{$t('navbar.messageUs')}}&nbsp;
      <a
        href="mailto:desarrollos@axtec.com.ar"
      >desarrollos@axtec.com.ar</a>
    </span>

    <modalNotification/>
    <notificationsNavbar class="col nav-item"/>
    <language-dropdown class="col nav-item"/>
    <status-bar class="col-md-2 nav-item"/>
  </vuestic-navbar>
</template>

<script>
import VuesticIconVuestic from "../../../vuestic-theme/vuestic-components/vuestic-icon/VuesticIconVuestic";
import VuesticNavbar from "../../../vuestic-theme/vuestic-components/vuestic-navbar/VuesticNavbar";
import HeaderSelector from "./components/HeaderSelector";
import LanguageDropdown from "./components/dropdowns/LanguageDropdown";
import ProfileDropdown from "./components/dropdowns/ProfileDropdown";
import MessageDropdown from "./components/dropdowns/MessageDropdown";
import modalNotification from "../../cansat/notifications/notifications";
import notificationsNavbar from "../../cansat/notifications/notificationsNavbar";
import statusBar from "../../cansat/notifications/statusbar";

export default {
  name: "app-navbar",

  components: {
    VuesticIconVuestic,
    VuesticNavbar,
    HeaderSelector,
    MessageDropdown,
    LanguageDropdown,
    ProfileDropdown,
    modalNotification,
    notificationsNavbar,
    statusBar
  },
  props: {
    isOpen: {
      type: Boolean,
      required: true
    }
  },
  computed: {
    valueProxy: {
      get() {
        return this.isOpen;
      },
      set(opened) {
        this.$emit("toggle-menu", opened);
      }
    }
  }
};
</script>
