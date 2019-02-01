<template>
  <div class="form-wizard-page">
    <div class="row">
      <div class="col-md-12">
        <p class="pt-3 mb-0" v-if="debug">
            <button class="btn btn-warning btn-micro float-right" @click="add()">
              {{ $t('cards.go') }}
              <span class="glyphicon glyphicon-arrow-right"></span>
            </button>
          </p>
        <vuestic-widget :headerText="$t('cansat.notifications.center.title')">
          <vuestic-data-table
            :apiUrl="apiUrl"
            :tableFields="tableFields"
            :itemsPerPage="itemsPerPage"
            :sortFunctions="sortFunctions"
            :apiMode="apiMode"
            :paginationPath="paginationPath"
            :tableData="tableData"
            ref="vuetable"
            :filterInputLabel="filterInputLabel"
            :itemsPerPageLabel="itemsPerPageLabel"
            :perPageSelectorShown="false"
          >
            <spring-spinner
              slot="loading"
              :animation-duration="2500"
              :size="70"
              color="#4ae387"
            />
          </vuestic-data-table>
      </vuestic-widget>
      </div>
    </div>
  </div>
</template>

<script>
import { SpringSpinner } from 'epic-spinners'
import FieldsDef from 'vuestic-components/vuestic-datatable/data/fields-definition'
import ItemsPerPageDef from 'vuestic-components/vuestic-datatable/data/items-per-page-definition'
import QueryParams from 'vuestic-components/vuestic-datatable/data/query-params'
import utils from '../../../services/utils'
import { mapGetters } from 'vuex'

export default {
  name: 'Table-Notifications',
  components: {
    SpringSpinner
  },
  data () {
    return {
      debug: this.$store.getters.axtec.debug,
      tableData:{ 'data': this.$store.getters.axtec.notificationsModal.slice().reverse()},
      data: {},
      apiUrl: '',
      apiMode: false, // Choose api mode or just pass array in data-table component
      filterInputLabel: this.$t('cansat.notifications.center.searchLabel'),
      itemsPerPageLabel: this.$t('cansat.notifications.center.perPageLabel'),
      tableFields: [
        {
          name: 'title', // Object property name in your data e.g. (data[0].name)
          title: this.$t('cansat.notifications.center.table.title'), // Object property name in your data which will be used for sorting
        },
        {
          name: 'code',
          title: this.$t('cansat.notifications.center.table.code') // Title of column
        },
        {
          name: 'type',
          title: this.$t('cansat.notifications.center.table.type') // Title of column
        },
        {
          name: 'date',
          title: this.$t('cansat.notifications.center.table.date')
        }
      ],
      itemsPerPage: [  // values in dropdown "Items Per Page"
        {
          value: 5
        }
      ],
      sortFunctions: {       // use custom sorting functions for prefered fields
        'date': function (item1, item2) {
          return item1 >= item2 ? 1 : -1
        }
      },
      paginationPath: ''
      }
  },
  created(){

  },

  computed: {
    newNotification(){
      return this.$store.getters.axtec.notificationsModal
    }
  },

  watch:{
    newNotification(newV){  
      this.tableData.data = newV.slice().reverse()
      this.$refs.vuetable.refresh() // https://github.com/ratiw/vuetable-2/issues/205
    }
  },

  methods:{
    add(){
        this.$store.commit('pushNotificationModal',{ 
            'title': this.$t('cansat.notifications.modal.project.titleNew'), 
            'date': utils.getDate(),
            'content': this.$t('cansat.notifications.modal.project.contentNew'),
            'code': 0,
            'okCallback': this.openDialog,
            'okText': this.$t('cansat.notifications.modal.okBtn'),
            'cancelText': this.$t('cansat.notifications.modal.cancelBtn'),
            'uuid': utils.generateUUID().toString(),
            'type': this.$t('cansat.notifications.center.types.info')
        })
    },
  } 
}
</script>

<style lang="scss">
.table {
  .table-axtec{
    background-color: $brand-primary;
  }
}
  .color-icon-label-table {
    td:first-child {
      width: 1rem;
    }
  }
</style>