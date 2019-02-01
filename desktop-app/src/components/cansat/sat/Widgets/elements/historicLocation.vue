<template>
    <div class="col-md-12">
        <vuestic-data-table style="padding:1rem;"
        :apiUrl="apiUrl"
        :tableFields="tableFields"
        :itemsPerPage="itemsPerPage"
        :apiMode="apiMode"
        :paginationPath="paginationPath"
        :tableData="history"
        ref="vuetable"
        :filterInputLabel="filterInputLabel"
        :itemsPerPageLabel="itemsPerPageLabel"
        :perPageSelectorShown="false"
        :noDataTemplate="''"
        >
        </vuestic-data-table>
    </div>
</template>

<script>
import { mapGetters } from 'vuex'

export default {
    name: 'historic-location',
    data () {
        return {
            data: {},
            apiUrl: '',
            apiMode: false,
            filterInputLabel: this.$t('cansat.notifications.center.searchLabel'),
            itemsPerPageLabel: this.$t('cansat.notifications.center.perPageLabel'),
            tableFields: [
                {
                name: 'date',
                title: this.$t('cansat.resources.sensors.tabs.location.table.date')
                },
                {
                name: 'lat', 
                title: this.$t('cansat.resources.sensors.tabs.location.table.latitude'), 
                },
                {
                name: 'lng',
                title: this.$t('cansat.resources.sensors.tabs.location.table.longitude') 
                }
            ],
            itemsPerPage: [ 
                {
                    value: 7
                }
            ],
            paginationPath: '',
            history: { 'data': this.$store.getters.axtec.project.cansat[0].location.history.slice().reverse()}
        }
    },

    computed: {
        newLocation(){
            return this.$store.getters.axtec.project.cansat[0].location.history
        }
    },

    watch:{
        newLocation(newV){  
            this.history.data = newV.slice().reverse()
            this.$refs.vuetable.refresh() // https://github.com/ratiw/vuetable-2/issues/205
        }
    }
}
</script>
