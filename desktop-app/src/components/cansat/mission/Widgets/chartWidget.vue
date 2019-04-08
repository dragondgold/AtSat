<template>
  <div class="form-elements chart-widget">
    <div v-if="!showAllSamples" class="col-md-12" style="text-align:center;">
      <h4 class="mb-0">{{ $t(sensor.type)}}</h4>
      <div class="col-md-12">
        <small>
          {{$t('cansat.mission.dashboard.charts.lastSamples')}}
          <a
            href
            @click.prevent="goToFullChart"
          >{{$t('cansat.mission.dashboard.charts.goToFullChart')}}</a>
        </small>
      </div>
    </div>
    <div class="col-md-12">
      <vue-chart
        style="min-height:15rem"
        ref="chart"
        :data="data"
        :options="options"
        :update-config="{duration: 800, easing: 'easeOutBounce'}"
        :type="type"
      />

      <a v-if="debug" href @click.prevent="addBtn">Agregar muestra</a>
    </div>
  </div>
</template>

<script>
import VueChart from "vuechart";
import utils from "services/utils";

export default {
  name: "chart-widget",
  components: {
    [VueChart.name]: VueChart
  },
  props: {
    sensor: {
      type: Object,
      required: true
    },
    showAllSamples: {
      type: Boolean,
      required: true,
      default: false
    }
  },
  data() {
    return {
      debug: this.$store.getters.axtec.debug,
      limit: 25,
      type: "line",
      dataOrig: {
        x: [],
        y: [],
        z: [],
        time: []
      },
      data: {
        labels: [],
        datasets: [
          {
            label: "",
            data: [],
            borderWidth: 2,
            ...(this.sensor._type != "vector"
              ? { fill: true }
              : { fill: false }),
            backgroundColor: "rgba(0, 255, 0, 0.1)",
            borderColor: "rgba(0, 255, 0, 1)",
            pointBackgroundColor: "rgba(0, 255, 0, 1)",
            pointBorderWidth: 0
          },
          {
            label: "",
            data: [],
            borderWidth: 2,
            fill: false,
            ...(this.sensor._type != "vector" ? { hidden: true } : []),
            borderColor: "rgba(0, 0, 255, 1)",
            pointBackgroundColor: "rgba(0, 0, 255, 1)",
            pointBorderWidth: 0
          },
          {
            label: "",
            data: [],
            borderWidth: 2,
            fill: false,
            ...(this.sensor._type != "vector" ? { hidden: true } : []),
            borderColor: "rgba(255, 0, 0, 1)",
            pointBackgroundColor: "rgba(255, 0, 0, 1)",
            pointBorderWidth: 0
          }
        ]
      },
      options: {
        fill: false,
        responsive: true,
        maintainAspectRatio: false,
        tooltips: {
          callbacks: {
            label: this.labelFormatter.bind(this)
          }
        },
        legend: {
          display: true,
          position: "bottom"
        },
        scales: {
          yAxes: [
            {
              scaleLabel: {
                display: true,
                labelString: "[ " + this.sensor.unit + " ]"
              }
            }
          ],
          xAxes: [
            {
              scaleLabel: {
                display: true,
                labelString: this.$t("cansat.mission.dashboard.charts.time")
              }
            }
          ]
        }
      }
    };
  },
  mounted() {
    this.loadData();
  },
  computed: {
    finished() {
      return this.$store.getters.axtec.project.cansat[0].missionFinish;
    },
    newSample() {
      return this.sensor.samples;
    }
  },
  watch: {
    newSample(samples) {
      if (samples.length != this.dataOrig.x.length) {
        let sample = samples[samples.length - 1];
        let x = undefined;
        let y = undefined;
        let z = undefined;
        let timespan = sample.timespan;
        if (this.sensor._type != "vector") {
          x = sample.lastValue;
          y = this.sensor.minValue;
          z = this.sensor.maxValue;
        } else {
          x = sample.x;
          y = sample.y;
          z = sample.z;
        }
        this.addNewData(x, y, z, timespan);
      }
    }
  },
  methods: {
    labelFormatter(t, d) {
      return t.yLabel.toString() + " " + this.sensor.unit;
    },
    loadData() {
      // Set label to each axis
      if (this.sensor._type != "vector") {
        this.data.datasets[0].label = this.$t(
          "cansat.mission.dashboard.charts.samples"
        );
        this.data.datasets[1].label = "Min";
        this.data.datasets[2].label = "Max";
      } else {
        this.data.datasets[0].label = "x";
        this.data.datasets[1].label = "y";
        this.data.datasets[2].label = "z";
      }

      // Set data to each axis
      for (let s = 0; s < this.sensor.samples.length; s++) {
        if (this.sensor._type != "vector") {
          this.addNewData(
            this.sensor.samples[s].lastValue,
            this.sensor.minValue,
            this.sensor.maxValue,
            this.sensor.samples[s].timespan
          );
        } else {
          this.addNewData(
            this.sensor.samples[s].x,
            this.sensor.samples[s].y,
            this.sensor.samples[s].z,
            this.sensor.samples[s].timespan
          );
        }
      }
    },
    addNewData(x, y, z, time) {
      if (x != undefined) {
        // 'x' or 'lastValue' depend of sensors _type
        this.dataOrig.x.push(x);
        if (this.dataOrig.x.length > this.limit && !this.showAllSamples) {
          this.data.datasets[0].data = this.dataOrig.x.slice(
            Math.max(this.dataOrig.x.length - this.limit, 0)
          );
        } else {
          this.data.datasets[0].data.push(x);
        }
      }
      if (y != undefined) {
        this.dataOrig.y.push(y);
        if (this.dataOrig.y.length > this.limit && !this.showAllSamples) {
          this.data.datasets[1].data = this.dataOrig.y.slice(
            Math.max(this.dataOrig.y.length - this.limit, 0)
          );
        } else {
          this.data.datasets[1].data.push(y);
        }
      }
      if (z != undefined) {
        this.dataOrig.z.push(z);
        if (this.dataOrig.z.length > this.limit && !this.showAllSamples) {
          this.data.datasets[2].data = this.dataOrig.z.slice(
            Math.max(this.dataOrig.z.length - this.limit, 0)
          );
        } else {
          this.data.datasets[2].data.push(z);
        }
      }

      if (time != undefined) {
        var hhmmss = time.split(":");
        time = hhmmss[1] + ":" + hhmmss[2];
        this.dataOrig.time.push(time);
        if (this.dataOrig.time.length > this.limit && !this.showAllSamples) {
          this.data.labels = this.dataOrig.time.slice(
            Math.max(this.dataOrig.time.length - this.limit, 0)
          );
        } else {
          this.data.labels.push(time);
        }
      }
    },
    addBtn() {
      let x;
      let y;
      let z;
      let time = utils.getDate();
      if (this.sensor._type != "vector") {
        x = Math.random() * 12;
      } else {
        x = Math.random() * 12;
        y = Math.random() * 12;
        z = Math.random() * 12;
      }

      let index = -1;
      let sensors = this.$store.getters.axtec.project.mission.data.sensors;
      for (let s = 0; s < sensors.length; s++) {
        if (sensors[s].id == this.sensor.id) {
          index = s;
          break;
        }
      }

      if (index != -1) {
        this.$store.commit("setSensor", {
          cansatIndex: 0,
          sensorIndex: index,
          lastValue: x,
          x: x,
          y: y,
          z: z
        });
        this.$store.commit("addSensorSample", {
          index: index,
          samples: {
            lastValue: x,
            x: x,
            y: y,
            z: z,
            timespan: time
          }
        });
      } else {
        alert("Index error pushing data");
      }
    },
    goToFullChart() {
      this.$router.push({
        name: "fullChart",
        params: { sensor: this.sensor, showAllSamples: true }
      });
    }
  }
};
</script>

