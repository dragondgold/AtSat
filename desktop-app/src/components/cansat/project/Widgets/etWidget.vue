<template>
    <div class="form-wizard-tab-content">
        <div> <h4>{{'cansat.project.new.wizard.stepTwo.description' | translate}}</h4></div>
        <div><p>{{ $t('cansat.project.new.wizard.stepTwo.titlePort') }}</p>  </div>
                <vuestic-simple-select
                :label="'cansat.project.new.wizard.stepTwo.selectPort' | translate"
                v-model="USBPort"
                name="USBPort"
                :required="true"
                ref="USBPort"
                v-bind:options="USBList"
                @input="validateConnection"
                disabled>
        </vuestic-simple-select>
    </div>
</template>

<script>
export default {
    name: 'et-widget',
    data () {
        return {
            USBPort: '',
            USBList: [ 'com1', 'com2'],
            valid: false
        }
    },
    computed:{

    },
    methods:{
        validateConnection(){
            if(this.USBPort != ''){
                this.valid = true
                this.isValid(this.valid)
            }else{
                this.valid = false
                this.isValid(this.valid)
            }
        },
        isValid(){
            return this.valid
        }
    }
}
</script>

