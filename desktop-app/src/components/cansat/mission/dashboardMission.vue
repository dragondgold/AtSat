<template>
  <div class="form-wizard-page">
    <div class="row">
      <div class="col-md-12">
        <vuestic-widget class="no-h-padding" :headerText="$t('cansat.mission.dashboard.title')">
          <Highcharts ref="highcharts" :options="options"/>
          <a href @click.prevent="addSample">Agregar muestra</a>

          <div v-if="(isProjectCreated && isTestOk) || isImported">
            <div class="form-wizard-tab-content">
              <controls-widget></controls-widget>
              <div v-if="isMissionActive || finished" class="row">
                <div class="col-md-6" v-for=" (sensor,key) in filterSensors()" :key="key">
                  <chart-widget :sensor="sensor" :showAllSamples="false"></chart-widget>
                </div>
                <div class="col-md-12">
                  <div class="collapse-page__content">
                    <div class="col-md-12" style="text-align:center">
                      <h4>{{ $t('cansat.resources.sensors.gps.title')}}</h4>
                    </div>
                    <location-sat></location-sat>
                  </div>
                </div>
              </div>
            </div>
          </div>
          <div v-else>
            <div v-if="!isProjectCreated" class="col-md-12" style="text-align:center;">
              <p>{{$t('cansat.link.projectNoCreated')}}</p>
              <div class="pt-1 mb-3" style="text-align:center">
                <button
                  class="btn btn-success btn-micro"
                  @click="goToNewProject()"
                >{{'cansat.project.new.wizard.title' | translate }}</button>
              </div>
            </div>
            <div v-else class="col-md-12" style="text-align:center;">
              <p>{{$t('cansat.test.noTestOk')}}</p>
              <div class="pt-1 mb-3" style="text-align:center">
                <button
                  class="btn btn-success btn-micro"
                  @click="goToTest()"
                >{{'cansat.test.title' | translate }}</button>
              </div>
            </div>
          </div>
        </vuestic-widget>
      </div>
    </div>
  </div>
</template>

<script>
import { genComponent } from "vue-highcharts";
import Highcharts from "highcharts";
import chartWidget from "./Widgets/chartWidget";
import controlsWidget from "./Widgets/controlsWidget";
import locationSat from "components/cansat/sat/locationSat";

var data = {
  chart: {
    zoomType: "x",
    type: "spline",
    animation: Highcharts.svg, // don't animate in old IE
    marginRight: 10,
    events: {
      load: function() {
        // set up the updating of the chart each second
        var series = this.series[0];
        setInterval(function() {
          var x = new Date().getTime(), // current time
            y = Math.random();
          series.addPoint([x, y], true, true);
        }, 1000);
      }
    }
  },

  time: {
    useUTC: false
  },

  title: {
    text: "Live random data"
  },
  xAxis: {
    type: "datetime",
    tickPixelInterval: 150
  },
  yAxis: {
    title: {
      text: "Value"
    },
    plotLines: [
      {
        value: 0,
        width: 1,
        color: "#808080"
      }
    ]
  },
  tooltip: {
    headerFormat: "<b>{series.name}</b><br/>",
    pointFormat: "{point.x:%Y-%m-%d %H:%M:%S}<br/>{point.y:.2f}"
  },
  legend: {
    enabled: false
  },
  exporting: {
    enabled: true
  },
  series: [
    {
      name: "Random data",
      data: (function() {
        // generate an array of random data
        var data = [],
          time = new Date().getTime(),
          i;

        for (i = -19; i <= 0; i += 1) {
          data.push({
            x: time + i * 1000,
            y: Math.random()
          });
        }
        return data;
      })()
    }
  ]
};

export default {
  name: "dashboard-mission",
  data() {
    return {
      sensors: this.filterSensors(),
      options: data
    };
  },
  components: {
    controlsWidget,
    chartWidget,
    locationSat,
    Highcharts: genComponent("Highcharts", Highcharts)
  },
  computed: {
    isProjectCreated() {
      return this.$store.getters.axtec.project.path != "";
    },
    isTestOk() {
      return this.$store.getters.axtec.project.cansat[0].testOk;
    },
    isImported() {
      return (
        this.$store.getters.axtec.project.cansat[0].missionType == "imported"
      );
    },
    isMissionActive() {
      return this.$store.getters.axtec.project.cansat[0].missionActive;
    },
    finished() {
      return this.$store.getters.axtec.project.cansat[0].missionFinish;
    }
  },
  methods: {
    filterSensors() {
      let filtered = this.$store.getters.axtec.project.mission.data.sensors.filter(
        function(n) {
          return n._type != "power";
        }
      );
      return filtered;
    },
    goToNewProject() {
      this.$router.push({ name: "newProject" });
    },
    goToTest() {
      this.$router.push({ name: "testSat" });
    },
    addSample() {
      this.options.series[0].data.push(Math.random());
    }
  }
};
</script>
