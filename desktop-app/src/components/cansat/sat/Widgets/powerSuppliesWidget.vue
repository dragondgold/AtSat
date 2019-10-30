<template>
    <div class="form-wizard-tab-content">
        <variables-tab :filterTableByType="'power'"/>
    </div>
</template>

<script>
import variablesTab from './variablesTab'

export default {
    name: 'power-supplies-widget',
    components:{
        variablesTab
    },
}
</script>