<template>
    <div class="form-elements chart-widget">
        <div class="col-md-12">
            <vue-chart style="display:block"
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
            type: 'line',
            data: {
                labels: ['','','','',''],
                datasets: [{
                    label: '# of Votes',
                    data: [10, 19, 3, 5, 2],
                    borderWidth: 3,
                    fill:false,
                    borderColor: 'rgba(0, 255, 0, 1)'},
                    {
                    label: 'Thresholh min',
                    data: [3, 3, 3, 3, 3],
                    borderWidth: 3,
                    fill:true,
                    borderColor: 'rgba(255, 0, 0, 1)'},
                    {
                    label: 'Thresholh max',
                    data: [23, 23, 23, 23, 23],
                    borderWidth: 3,
                    fill:true,
                    borderColor: 'rgba(255, 0, 0, 1)'}
                    
                    ]
            },
            options: {
                fill: false,
                scales: {
                    yAxes: [{
                        ticks: {
                            beginAtZero:true
                        }
                    }]
                }
            },
            newVal: 0
        }
    },
    methods:{
        add(){
            this.newVal = 12
            this.data.datasets[0].data.push(this.newVal)
            this.data.labels.push(this.newVal)
            this.data.datasets[1].data.push(3)

        }
    }
}
</script>

