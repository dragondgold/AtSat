<template>
  <div class="notification-dropdown"  @click="goToNotifications()" >
    <span class="fa fa-bell bell-notification-navbar"/>
  </div>
</template>

<script>
import ProjectManager from 'services/projectManager'

export default {
  name: 'notifications-navbar',
  data () {
    return {
      isShown: false,
    }
  },
  methods:{
    goToNotifications(){
      this.$router.push({name:'notificationsCenter'})
    }
  }
}
</script>

<style lang="scss">
@import '../../../vuestic-theme/vuestic-sass/resources/resources';

.notification-dropdown {
  @include flex-center();
  cursor: pointer;

  .bell-notification-navbar {
    font-size: 2rem;
    color: $brand-primary;
  }
}
</style>
