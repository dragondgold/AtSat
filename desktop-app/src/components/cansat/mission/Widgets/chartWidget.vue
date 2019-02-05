<template>
    <div class="form-elements chart-widget">
        <div class="col-md-12">
            <vue-chart style="min-height:15rem"
                :data="data"
                :options="options"
                :update-config="{duration: 800, easing: 'easeOutBounce'}"
                :type="type"
                />
            <div><button class="btn btn-micro btn-success" @click="add()">
        add
        </button>
                
        </div>

    
    
        
    </div>
    </div>
    

</template>

<script>
import VueChart from 'vuechart'

export default {
    name: 'chart-widget',
    components: {
        [VueChart.name]: VueChart,
    },
    data () {
        return {
            
            dataOrig:{
                x: [23, 23, 23, 23, 23],
                y: [10, 19, 3, 5, 2],
                z: [3, 3, 3, 3, 3]
            },
            type: 'line',
            data: {
                labels: ['','','','',''],
                datasets: [{
                    label: '# of Votes',
                    data: [23, 23, 23, 23, 23],
                    borderWidth: 2,
                    fill:false,
                    borderColor: 'rgba(0, 255, 0, 1)',
                    pointBackgroundColor: 'rgba(0, 255, 0, 1)'},
                    {
                    label: 'Thresholh min',
                    data: [10, 19, 3, 5, 2],
                    borderWidth: 1,
                    fill:false,
                    borderColor: 'rgba(121, 121, 121, 1)'},
                    {
                    label: 'Thresholh max',
                    data: [3, 3, 3, 3, 3],
                    borderWidth: 1,
                    fill:false,
                    borderColor: 'rgba(0, 0, 0, 1)'}
                    
                    ]
            },
            options: {
                fill: false,
                responsive:true,
    maintainAspectRatio: false,
                legend: {
                        display: true,
                        position: 'bottom',
                    }
            }
        }
    },
    methods:{
        add(){
            this.newVal = Math.random() * 6
            this.dataOrig.x.push(this.newVal)
            this.dataOrig.y.push(3*this.newVal)
            this.dataOrig.z.push(20*this.newVal)
            
            let limit= 10
            if(this.dataOrig.x.length > limit){
                this.data.datasets[0].data = this.dataOrig.x.slice(Math.max(this.dataOrig.x.length - limit, 0))
                this.data.datasets[1].data = this.dataOrig.y.slice(Math.max(this.dataOrig.y.length - limit, 0))
                this.data.datasets[2].data = this.dataOrig.z.slice(Math.max( this.dataOrig.z.length - limit, 0))
            }else{
                
                this.data.datasets[0].data.push(this.dataOrig.x[this.dataOrig.x.length-1]) 
                this.data.labels.push('')
                this.data.datasets[1].data.push(this.dataOrig.y[this.dataOrig.y.length-1])
                this.data.datasets[2].data.push(this.dataOrig.z[this.dataOrig.z.length-1])
            }
        }
    }
}
</script>

