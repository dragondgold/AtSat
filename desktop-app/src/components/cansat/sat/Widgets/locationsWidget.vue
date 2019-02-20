<template>
    <div class="leaflet-map">
        <div class="leaflet-bottom leaflet-center">
            <vuestic-tooltip :options="{ 'content': $t('cansat.resources.sensors.gps.center'), 'placement': 'top'  }">
                <p class="pt-1 mb-3" style="text-align:center; pointer-events: auto;font-size:1.5rem;">
                    <button class="btn btn-success btn-with-icon btn-micro rounded-icon " style="vertical-align: middle;" @click="centerToMarker()">
                        <i class="fa fa-location-arrow"></i>
                    </button>
                </p>
            </vuestic-tooltip>  
        </div>         
    </div>
</template>

<script>
import 'leaflet-map'
import * as L from 'leaflet'
import { mapGetters } from 'vuex'

export default {
    data () {
        return {
            map: {},
            marker: {},
            minZoom: 7,
            maxZoom: 10,
            altitude: this.$store.getters.axtec.project.cansat[0].altitude
        }
    },

    computed:{
        ...mapGetters({
            location: 'location',
            signal: 'cansatSignal',
            name: 'cansatName',
            battery: 'cansatBattery'
        })
    },

    watch:{
        location: {
            handler: function(newValue) {
                this.map.panTo(new L.LatLng(this.location.lat,this.location.lng), this.minZoom)
                this.marker.setLatLng(new L.LatLng(this.location.lat,this.location.lng))
            },
            deep: true
        }
    },

    mounted(){
        this.mountMap()
    },
    methods:{
        mountMap(){
            //    L.Icon.Default.imagePath = 'assets/vendor/leaflet' TODO: make it work with webpack
            L.Icon.Default.imagePath = 'https://unpkg.com/leaflet@1.0.3/dist/images'

            this.map = new L.map(this.$el).setView([this.location.lat,this.location.lng], this.minZoom)

            this.map.locate({setView: true, watch: true, minZoom: this.minZoom, maxZoom: this.maxZoom})

            L.tileLayer('https://{s}.tile.osm.org/{z}/{x}/{y}.png', {
            attribution: '&copy; <a href="https://osm.org/copyright">OpenStreetMap</a> contributors'
            }).addTo(this.map)
            this.marker = new L.marker()
            this.marker.setLatLng([this.location.lat, this.location.lng])
            let popup = new L.popup({
                autoClose: false, closeOnClick:false
            }).setContent(
                '<div style="width:5rem">' +
                    this.name + 
                    '<br><i class="fa fa-rocket" style="color:red;margin:0.24rem"></i>: ' +  this.altitude +' m' + 
                    '<br><i class="fa fa-signal" style="color:blue;margin:0.24rem"></i>: '+ this.signal  + ' %' +
                    '<br><i class="fa fa-battery-full" style="color:green;margin:0.24rem"></i>: '+ this.battery + ' %' +
                '</div>'
            )

            this.marker.bindPopup(popup)
            this.marker.addTo(this.map)
            this.marker.openPopup()   
         
        },
        centerToMarker(){
            this.map.panTo([this.location.lat,this.location.lng])
            this.marker.openPopup()
        }
    }
}
</script>

<style lang="scss">
  @import "~leaflet/dist/leaflet.css";

  .leaflet-map {
    height: 100%;
  }
</style>