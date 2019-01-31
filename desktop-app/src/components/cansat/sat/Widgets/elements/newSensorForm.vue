<template>
    <div class="form-elements">
        <div class="row">
            <div class="col-md-12" style="text-align:center;">
                <div class="row ">
                    <div class="col-md-12" style="text-align:center"> 
                        <h4>{{ $t('cansat.resources.sensors.tabs.createdByUser.button')}}</h4>
                        <p> {{ $t('cansat.resources.sensors.tabs.createdByUser.newSensor.subtitle')}}</p>
                    </div>

                    <div class="col-md-4">
                        <div class="form-group">
                            <div class="input-group ">
                                <input id="type-input" v-model="newSensor.type" maxlength="20" required/>
                                <label class="control-label" for="type-input">
                                {{'cansat.resources.sensors.tabs.variables.table.type'| translate}}
                                </label>
                                <i class="bar"></i>
                                <small v-if="!isFieldValid(newSensor.type)" class="help text-danger">
                                {{$t('cansat.resources.sensors.tabs.createdByUser.newSensor.invalidType')}}
                                </small>
                            </div>                 
                        </div>
                    </div>
                    <div class="col-md-2">
                        <div class="form-group" :class="{'has-error': !isMinValid(), 'valid': isMinValid()}">
                            <div class="input-group">
                                <input id="min-input" type="number" v-model="newSensor.minValue" required/>
                                <label class="control-label" for="min-input">
                                {{'cansat.resources.sensors.tabs.variables.table.min'| translate}}
                                </label>
                                <i class="bar"></i>
                                <small v-if="!isMinValid()" class="help text-danger">
                                {{$t('cansat.resources.sensors.tabs.createdByUser.newSensor.invalidMin')}}
                                </small>
                            </div>
                        </div>
                    </div>
                    <div class="col-md-2">
                        <div class="form-group">
                            <div class="input-group">
                                <input id="max-input" type="number" v-model="newSensor.maxValue" required/>
                                <label class="control-label"  for="max-input">
                                {{'cansat.resources.sensors.tabs.variables.table.max'| translate}}
                                </label>
                                <i class="bar"></i>
                            </div>
                        </div>
                    </div>
                    <div class="col-md-4">
                        <div class="form-group">
                            <div class="input-group">
                                <input id="unit-input" v-model="newSensor.unit" maxlength="5" required/>
                                <label class="control-label" for="unit-input">
                                {{'cansat.resources.sensors.tabs.variables.table.unit'| translate}}
                                </label>
                                <i class="bar"></i>
                                <small v-if="!isFieldValid(newSensor.unit)" class="help text-danger">
                                {{$t('cansat.resources.sensors.tabs.createdByUser.newSensor.invalidUnit')}}
                                </small>
                            </div>              
                        </div>
                    </div> 
                    <div class="col-md-12" style="text-align:center;">
                        <button v-if="isFieldValid(newSensor.unit)&&isFieldValid(newSensor.type)&&isMinValid()" class="btn btn-micro btn-success" @click="addnewSensor()">
                        <span v-if="newSensor.id ==''">{{$t('cansat.resources.sensors.tabs.createdByUser.newSensor.add')}}</span>
                        <span v-else>{{$t('cansat.resources.sensors.tabs.createdByUser.newSensor.edit')}}</span>
                        </button>
                    </div>  

                </div>     
            </div>
        </div>
    </div>
</template>

<script>
import defaultSensors from 'data/Sensors'

export default {
    name: 'new-sensor-form',
    data () {
        return {
            newSensor:  JSON.parse(JSON.stringify(defaultSensors.newSensor)),
            minStep: 20
        }
    },
    methods: {
        clearFields(){
            this.newSensor = JSON.parse(JSON.stringify(defaultSensors.newSensor))
        },
        addnewSensor(){
            let createNew = true
            if(this.newSensor.id != ''){
                createNew = false
            }else{
                this.newSensor.id = this.getValidID() + 1 
            }
            this.newSensor.maxThreshold = this.newSensor.maxValue
            this.newSensor.minThreshold = this.newSensor.minValue
            this.newSensor.lastvalue = this.newSensor.minValue
            this.newSensor.status = 'danger'
            this.newSensor._type = 'user'
            this.newSensor.step = this.calculateStep()
            if(createNew){
                this.$store.commit('addNewSensor', this.newSensor)
            }else{
                this.$store.commit('setSensor', this.newSensor)
            }
            this.$emit('newSensorAdded')
            this.clearFields()
        },
        editSensor(s){
            this.newSensor = s 
            
        },
        removeSensor(s){
            this.$store.commit('removeSensor', { cansatIndex: 0, id: s.id  })
            this.$emit('sensorRemoved')
        },
        isMinValid(){
            return this.newSensor.minValue < this.newSensor.maxValue
        },
        isFieldValid(field){
            return field != ''
        },
        calculateStep(){
            return ((this.newSensor.maxValue -this.newSensor.minValue) / this.minStep)
        },
        getValidID(){
            return JSON.parse(JSON.stringify( this.$store.getters.axtec.project.cansat[0].sensors.length))
        }
    }
}
</script>
