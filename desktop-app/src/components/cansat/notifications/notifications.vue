<template>
    <div class="row">
        <modal v-bind:large="true" v-bind:force="true" ref="modal" :cancelText="cancelText" :cancelDisabled="cancelDisabled" :okText="okText" @ok="ok()" @cancel="cancel()">
            <div slot="title">{{ title }}</div>
            <div>
            {{ content }}
            </div>
        </modal>
    </div>
</template>

<script>
import Modal from '../../../vuestic-theme/vuestic-components/vuestic-modal/VuesticModal'
import store from '../../../store'
import utils from '../../../services/utils'

export default {
    name: 'notification-modal',
    data () {
        return {
            title: '',
            content: '',
            okText: 'Ok',
            cancelText: 'Cancel',
            indexModal: 0,
            indexToast: 0,
            isToastContentPresent: true,
            toastText: 'This toast is awesome!',
            toastDuration: 2500,
            toastIcon: 'fa-rocket',
            toastPosition: 'bottom-right',
            isToastFullWidth: false,
            cancelDisabled: false
        }
    },
    components: {
        Modal
    },
    methods: {
        showModal () {
            this.$refs.modal.open() // $refs property name same as in ref="modal"
        },
        ok(){
            if(this.$store.getters.axtec.notificationsModal[this.indexModal].okCallback !== undefined){
                this.$store.getters.axtec.notificationsModal[this.indexModal].okCallback()
            }
            
        },
        cancel(){
            if(this.$store.getters.axtec.notificationsModal[this.indexModal].cancelCallback !== undefined){
                this.$store.getters.axtec.notificationsModal[this.indexModal].cancelCallback()
            }
        },
        launchToast () {
            this.showToast(this.toastText, {
                icon: this.toastIcon,
                position: this.toastPosition,
                duration: this.toastDuration,
                fullWidth: this.isToastFullWidth 
            })
        }
    },  
    computed: {
        newNotificationModal () {
            return store.getters.axtec.notificationsModal
        },
        newNotificationToast () {
            return store.getters.axtec.notificationsToast
        }
    },
    watch: {
        newNotificationModal () {
            let lengthM = this.$store.getters.axtec.notificationsModal.length
            if(lengthM > 0 && this.$store.getters.axtec.notificationsModal[this.indexModal]){
                this.indexModal = lengthM-1
                this.title =  this.$t(this.$store.getters.axtec.notificationsModal[this.indexModal].title)
                this.content =  this.$t(this.$store.getters.axtec.notificationsModal[this.indexModal].content)
                this.okText =  this.$t(this.$store.getters.axtec.notificationsModal[this.indexModal].okText)
                this.cancelText = this.$t(this.$store.getters.axtec.notificationsModal[this.indexModal].cancelText) 
                this.cancelDisabled = this.$store.getters.axtec.notificationsModal[this.indexModal].cancelDisabled
                if(!this.$store.getters.axtec.notificationsModal[this.indexModal].type.includes('Info')){
                    this.showModal()
                    utils.log('Showing modal', this.$store.getters.axtec.notificationsModal[this.indexModal]) 
                }
                utils.log('Add modal to center', this.$store.getters.axtec.notificationsModal[this.indexModal]) 
            }
        },
        newNotificationToast () {
            let lengthT = this.$store.getters.axtec.notificationsToast.length
            if(lengthT > 0 && this.$store.getters.axtec.notificationsToast[this.indexToast]){
                this.indexToast = lengthT-1
                this.toastText =  this.$store.getters.axtec.notificationsToast[this.indexToast].text
                this.toastIcon = this.$store.getters.axtec.notificationsToast[this.indexToast].icon
                this.launchToast()
                utils.log('Showing toast', this.$store.getters.axtec.notificationsToast[this.indexToast]) 
            }
        }
    }
}
</script>

<style lang="scss" scoped>
  .toasted-container.sample-toasted-container {
    position: static;
    transform: translateX(0);

    .toasted {
      position: static;
      transform: translateY(0);
    }
  }
</style>