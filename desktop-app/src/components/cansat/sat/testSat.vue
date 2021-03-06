<template>
  <div class="form-wizard-page">
    <div class="row">
      <div class="col-md-12">
        <vuestic-widget :headerText="$t('cansat.test.title')">
          <div v-if="isConnected && isProjectCreated">
            <div v-if="!isTestRunning" class="col-md-12 mb-3" style="text-align:center;">
              <p>{{$t('cansat.test.description')}}</p>
              <button class="btn btn-micro btn-success" @click="startTest()">
                <span>{{$t('cansat.test.start')}}</span>
              </button>
            </div>
            <table class="table table-striped first-td-padding col-md-12">
              <thead>
                <tr>
                  <td v-for="(field,key) in fieldsSensors" :key="key">{{$t(field.title)}}</td>
                </tr>
              </thead>
              <tbody>
                <tr v-for="(test,key) in tests" :key="key">
                  <td style="width:90%">{{$t(test.name)}}</td>
                  <td>
                    <i
                      v-if="test.status == 'error'"
                      class="fa icon-table fa-exclamation-triangle icon-table-danger"
                    ></i>
                    <div v-if="test.status == 'testing'" class="flex sm12 md3">
                      <div class="spinner-box">
                        <BreedingRhombusSpinner
                          slot="loading"
                          :animation-duration="1500"
                          :size="30"
                          color="#00FF00"
                        />
                      </div>
                    </div>
                    <i v-if="test.status == 'ok'" class="fa icon-table fa-check icon-table-success"></i>
                  </td>
                </tr>
              </tbody>
            </table>
            <div v-if="testFinished">
              <h4 class="mb-2" style="text-align:center;">
                <span v-if="anErrorOcurred">{{$t('cansat.test.error') + $t(tests[actualTest].name)}}</span>
                <span v-else>{{$t('cansat.test.ok')}}</span>
              </h4>
              <div class="col-md-12 mt-2" style="height: 2rem;text-align:center;">
                <button class="btn btn-micro btn-info" style="float:left;" @click="goToResources()">
                  <span>{{$t('cansat.test.results')}}</span>
                </button>
                <div v-if="anErrorOcurred">
                  <button
                    class="btn btn-micro btn-danger"
                    style="float:right;"
                    @click="goToMissionAnyway()"
                  >
                    <span>{{$t('cansat.test.goToMissionAnyway')}}</span>
                  </button>
                </div>
                <div v-else>
                  <button
                    class="btn btn-micro btn-success"
                    style="float:right;"
                    @click="goToMission()"
                  >
                    <span>{{$t('cansat.test.goToMission')}}</span>
                  </button>
                </div>
              </div>
              <div class="col-md-12 mt-2" style="text-align:center;">
                <small>{{$t('cansat.test.tips')}}</small>
              </div>
            </div>
          </div>
          <div v-else>
            <div v-if="!isProjectCreated" style="text-align:center;">
              <p>{{$t('cansat.link.projectNoCreated')}}</p>
              <p class="pt-1 mb-3" style="text-align:center">
                <button
                  class="btn btn-success btn-micro"
                  @click="goToNewProject()"
                >{{'cansat.project.new.wizard.title' | translate }}</button>
              </p>
            </div>
            <div v-else style="text-align:center;">
              <p>{{$t('cansat.link.cansatDisconnectedLabel')}}</p>
              <p class="pt-1 mb-3" style="text-align:center">
                <button class="btn btn-success btn-micro" @click="goToLink()">
                  {{'cansat.link.title' | translate }}
                  <span class="fa fa-link"></span>
                </button>
              </p>
            </div>
          </div>
        </vuestic-widget>
      </div>
    </div>
  </div>
</template>

<script>
import CanSatAPI from "services/CanSatAPI";
import utils from "services/utils";
import { BreedingRhombusSpinner } from "epic-spinners";
import { truncate, truncateSync } from "fs";

const powerSuppliesArray = [1, 2, 3, 4, 5, 6];
const sensorsArray = [8, 9, 10, 12, 13, 14, 15];
const gpsArray = [7];

let that;

export default {
  name: "test-sat",
  components: {
    BreedingRhombusSpinner
  },
  data() {
    return {
      debug: this.$store.getters.axtec.debug,
      isConnected: this.$store.getters.axtec.project.cansat[0].connected,
      testResult: this.$store.getters.axtec.project.cansat[0].testOk,
      testTimetoout: 2000,

      fieldsSensors: [
        { title: "cansat.test.table.test" },
        { title: "cansat.test.table.status" }
      ],
      tests: [
        {
          name: "cansat.resources.actuators.title",
          status: "",
          test: this.testActuators,
          errorNotif: this.errorNotifActuators
        },
        {
          name: "cansat.resources.sensors.tabs.powerSupplies.title",
          status: "",
          test: this.testsPowerSupllies,
          errorNotif: this.errorNotifPowerSupplies
        },
        {
          name: "cansat.resources.sensors.title",
          status: "",
          test: this.testsSensors,
          errorNotif: this.errorNotifSensors
        },
        {
          name: "cansat.resources.sensors.gps.title",
          status: "",
          test: this.testGPS,
          errorNotif: this.errorNotifGPS
        }
      ],
      actualTest: 0,
      isTestRunning: false,
      testFinished: false,
      anErrorOcurred: false,
      testingName: "",
      gpsHistory: 0
    };
  },

  created() {
    that = this;
  },

  computed: {
    isCanSatConnected() {
      return this.$store.getters.axtec.project.cansat[0].connected;
    },
    isTestOk() {
      return this.$store.getters.axtec.project.cansat[0].testOk;
    },
    isProjectCreated() {
      return this.$store.getters.axtec.project.path != "";
    }
  },

  watch: {
    isCanSatConnected(changes) {
      this.isConnected = changes;
    },
    isTestOk(changes) {
      this.testResult = changes;
    }
  },

  methods: {
    startTest() {
      this.clearStatus();
      this.testFinished = false;
      this.isTestRunning = true;
      this.anErrorOcurred = false;
      this.actualTest = 0;
      this.tests[this.actualTest].test();
    },
    testActuators() {
      this.tests[this.actualTest].status = "testing";
      this.testParachute();
    },
    testParachute() {
      this.testingName = "parachute";
      this.$store.commit("pushNotificationModal", {
        title: this.$t("cansat.notifications.modal.test.parachute"),
        date: utils.getDate(),
        content: this.$t("cansat.notifications.modal.test.parachuteContent"),
        code: 0,
        okCallback: this.okCallbackActuators,
        cancelCallback: this.errorCallbackActuators,
        okText: this.$t("cansat.notifications.modal.yesBtn"),
        cancelText: this.$t("cansat.notifications.modal.noBtn"),
        uuid: utils.generateUUID().toString(),
        type: this.$t("cansat.notifications.center.types.action")
      });
      CanSatAPI.setActuator(0, 1);
    },
    testBalloon() {
      this.testingName = "balloon";
      this.$store.commit("pushNotificationModal", {
        title: this.$t("cansat.notifications.modal.test.balloon"),
        date: utils.getDate(),
        content: this.$t("cansat.notifications.modal.test.balloonContent"),
        code: 0,
        okCallback: this.okCallbackActuators,
        cancelCallback: this.errorCallbackActuators,
        okText: this.$t("cansat.notifications.modal.yesBtn"),
        cancelText: this.$t("cansat.notifications.modal.noBtn"),
        uuid: utils.generateUUID().toString(),
        type: this.$t("cansat.notifications.center.types.action")
      });
      CanSatAPI.setActuator(1, 1);
    },
    okCallbackActuators() {
      if (that.testingName == "parachute") {
        that.testBalloon();
      } else {
        that.tests[that.actualTest].status = "ok";
        that.actualTest++;
        setTimeout(that.tests[that.actualTest].test(), that.testTimetoout);
      }
    },
    errorCallbackActuators() {
      that.error();
    },
    errorNotifActuators() {
      if (this.testingName == "parachute") {
        this.$store.commit("pushNotificationModal", {
          title: this.$t("cansat.notifications.modal.test.parachuteError"),
          content: this.$t(
            "cansat.notifications.modal.test.actuatorsContentError"
          ),
          date: utils.getDate(),
          code: 0,
          okText: this.$t("cansat.notifications.modal.okBtn"),
          uuid: utils.generateUUID().toString(),
          type: this.$t("cansat.notifications.center.types.error"),
          cancelDisabled: true
        });
      } else {
        this.$store.commit("pushNotificationModal", {
          title: this.$t("cansat.notifications.modal.test.balloonError"),
          content: this.$t(
            "cansat.notifications.modal.test.actuatorsContentError"
          ),
          date: utils.getDate(),
          code: 0,
          okText: this.$t("cansat.notifications.modal.okBtn"),
          uuid: utils.generateUUID().toString(),
          type: this.$t("cansat.notifications.center.types.error"),
          cancelDisabled: true
        });
      }
    },

    testsPowerSupllies() {
      this.$store.commit("pushNotificationModal", {
        title: this.$t("cansat.notifications.modal.test.powerSupplies"),
        date: utils.getDate(),
        code: 0,
        uuid: utils.generateUUID().toString(),
        type: this.$t("cansat.notifications.center.types.info")
      });
      this.tests[this.actualTest].status = "testing";
      CanSatAPI.getSensors(
        powerSuppliesArray,
        that.okCallbackPowerSupplies,
        that.errorCallbackPowerSupplies
      );
    },
    okCallbackPowerSupplies() {
      that.tests[that.actualTest].status = "ok";
      that.actualTest++;
      setTimeout(that.tests[that.actualTest].test(), that.testTimetoout);
    },
    errorCallbackPowerSupplies() {
      that.error();
    },
    errorNotifPowerSupplies() {
      this.$store.commit("pushNotificationModal", {
        title: this.$t("cansat.notifications.modal.test.powerSuppliesError"),
        content: this.$t(
          "cansat.notifications.modal.test.powerSuppliesContentError"
        ),
        date: utils.getDate(),
        code: 0,
        okText: this.$t("cansat.notifications.modal.okBtn"),
        uuid: utils.generateUUID().toString(),
        type: this.$t("cansat.notifications.center.types.error"),
        cancelDisabled: true
      });
    },

    testsSensors() {
      this.$store.commit("pushNotificationModal", {
        title: this.$t("cansat.notifications.modal.test.sensors"),
        date: utils.getDate(),
        code: 0,
        uuid: utils.generateUUID().toString(),
        type: this.$t("cansat.notifications.center.types.info")
      });
      this.tests[this.actualTest].status = "testing";
      CanSatAPI.getSensors(
        sensorsArray,
        that.okCallbackSensors,
        that.errorCallbackSensors
      );
    },
    okCallbackSensors() {
      that.tests[that.actualTest].status = "ok";
      that.actualTest++;
      setTimeout(that.tests[that.actualTest].test(), that.testTimetoout);
    },
    errorCallbackSensors() {
      that.error();
    },
    errorNotifSensors() {
      this.$store.commit("pushNotificationModal", {
        title: this.$t("cansat.notifications.modal.test.sensorsError"),
        content: this.$t("cansat.notifications.modal.test.sensorsContentError"),
        date: utils.getDate(),
        code: 0,
        okText: this.$t("cansat.notifications.modal.okBtn"),
        uuid: utils.generateUUID().toString(),
        type: this.$t("cansat.notifications.center.types.error"),
        cancelDisabled: true
      });
    },

    testGPS() {
      that.gpsHistory =
        that.$store.getters.axtec.project.mission.data.location.history.length;
      that.$store.commit("pushNotificationModal", {
        title: this.$t("cansat.notifications.modal.test.gps"),
        date: utils.getDate(),
        code: 0,
        uuid: utils.generateUUID().toString(),
        type: this.$t("cansat.notifications.center.types.info")
      });

      that.tests[that.actualTest].status = "testing";
      CanSatAPI.getSensors(gpsArray, that.okCallbackGPS, that.errorCallbackGPS);
    },
    okCallbackGPS() {
      setTimeout(() => {
        that.tests[this.actualTest].status = "ok";
        that.finish();
        that.$store.commit("pushNotificationModal", {
          title: this.$t("cansat.notifications.modal.test.finished"),
          date: utils.getDate(),
          code: 0,
          uuid: utils.generateUUID().toString(),
          type: this.$t("cansat.notifications.center.types.info")
        });
        that.$store.commit("setTestStatus", {
          cansatIndex: 0,
          testOk: true
        });
        that.$store.commit("pushNotificationToast", {
          text: this.$t("cansat.test.ok"),
          icon: "fa-check"
        });
      }, 1000);
    },
    errorCallbackGPS() {
      that.error();
    },
    errorNotifGPS() {
      that.$store.commit("pushNotificationModal", {
        title: this.$t("cansat.notifications.modal.test.gpsError"),
        content: this.$t("cansat.notifications.modal.test.gpsContentError"),
        date: utils.getDate(),
        code: 0,
        okText: this.$t("cansat.notifications.modal.okBtn"),
        uuid: utils.generateUUID().toString(),
        type: this.$t("cansat.notifications.center.types.error"),
        cancelDisabled: true
      });
    },

    error() {
      this.tests[this.actualTest].status = "error";
      this.anErrorOcurred = true;
      this.finish();
      this.tests[this.actualTest].errorNotif();
      this.$store.commit("setTestStatus", {
        cansatIndex: 0,
        testOk: false
      });
      this.$store.commit("pushNotificationToast", {
        text:
          this.$t("cansat.test.error") +
          this.$t(this.tests[this.actualTest].name),
        icon: "fa-remove"
      });
    },
    finish() {
      this.testFinished = true;
      this.isTestRunning = false;
      clearTimeout(this.timeoutPromise);
    },
    clearStatus() {
      for (let t = 0; t < this.tests.length; t++) {
        this.tests[t].status = "";
      }
    },
    goToResources() {
      this.$router.push({ name: "resourcesSat" });
    },
    goToMission() {
      this.$router.push({ name: "newMission" });
    },
    goToMissionAnyway() {
      this.$store.commit("setTestStatus", {
        cansatIndex: 0,
        testOk: true
      });
      this.$router.push({ name: "newMission" });
    },
    goToLink() {
      this.$router.push({ name: "linkSat" });
    },
    goToNewProject() {
      this.$router.push({ name: "newProject" });
    }
  }
};
</script>

<style lang="scss">
.icon-table {
  font-size: 1rem;
  color: white;
  border-radius: 50%;
  padding: 0.5rem;
}
.icon-table-success {
  background-color: $brand-success;
}
.icon-table-info {
  background-color: $brand-info;
}
.icon-table-danger {
  background-color: $brand-danger;
}
</style>