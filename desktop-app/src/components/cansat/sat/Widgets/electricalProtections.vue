<template>
    <div class="form-wizard-tab-content">
        <table class="table table-striped first-td-padding col-md-12">
            <thead>
                <tr>
                    <td v-for="(field,key) in fields" :key="key">{{$t(field.title)}}</td>             
                </tr>
            </thead>
            <tbody>
                <tr v-for="(suply,key) in powerSuplies" :key="key">
                    <td>{{$t(suply.name)}}</td>
                    <td>{{suply.voltage}}</td>
                    <td>{{suply.current}}</td>
                    <td>{{suply.maxCurrent}}</td>
                    <td> 
                        <i class="fa icon-table" :class="[suply.status =='Active' ? 'fa-check icon-table-success' : 'fa-exclamation-triangle icon-table-danger']" ></i> 
                    </td>
                </tr>
            </tbody>
        </table>
    </div>
</template>

<script>
export default {
    name: 'electrical-protections',
    props: {
        enableWizard: {
            type: Boolean,
            default: true
        }
    },
    data () {
        return {
            fields: [
                { "title" : 'cansat.protections.tabs.electrical.table.name'},
                { "title" : 'cansat.protections.tabs.electrical.table.voltage'}, 
                { "title": 'cansat.protections.tabs.electrical.table.current'}, 
                { "title": 'cansat.protections.tabs.electrical.table.maxCurrent'}, 
                { "title" : 'cansat.protections.tabs.electrical.table.status'}],
            powerSuplies: this.$store.getters.axtec.project.cansat[0].protections.powerSupply
        }
    },
    created(){

    },
    methods:{

    }
}
</script>

<style lang="scss">
    .icon-table{
        border-radius: 50%;       
        color: white;
        padding: 0.35rem;
    }
    .icon-table-success {
        background: $brand-primary;
    }
    .icon-table-danger {
        background: $brand-danger;
    }
</style>