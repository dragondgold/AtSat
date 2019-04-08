<template>
  <div class="form-wizard-tab-content">
    <h4
      v-if="enableWizard"
      style="text-align:center"
    >{{'cansat.project.new.wizard.stepTwo.description' | translate}}</h4>
    <h4 v-else style="text-align:center">{{'cansat.link.tabs.et' | translate}}</h4>
    <div v-if="!isConnected" style="text-align:center">
      <p style="text-align:center">{{ $t('cansat.project.new.wizard.stepTwo.titlePort') }}</p>
      <vuestic-simple-select
        style="text-align:center"
        :label="'cansat.project.new.wizard.stepTwo.selectPort' | translate"
        v-model="USBPort"
        name="USBPort"
        :required="true"
        ref="USBPort"
        v-bind:options="serialPorts"
        disabled
      ></vuestic-simple-select>
    </div>
    <div class="col-md-12">
      <div>
        <p
          v-if="isConnected"
          style="text-align:center"
        >{{$t('cansat.link.etConnectedLabel') + port.toUpperCase()}}</p>
        <div class="col-md-12 pt-1 mb-3" style="text-align:center">
          <button v-if="!isConnected" class="btn btn-success btn-micro" @click="connect()">
            {{ $t('cansat.link.connect') }}
            <span class="fa fa-link"></span>
          </button>
          <button v-else class="btn btn-danger btn-micro" @click="disconnect()">
            {{ $t('cansat.link.disconnect') }}
            <span class="fa fa-unlink"></span>
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import CanSatAPI from "services/CanSatAPI";
import { debug } from "util";
export default {
  name: "et-widget",
  props: {
    enableWizard: {
      type: Boolean,
      default: true
    }
  },
  data() {
    return {
      USBPort: "",
      valid: false
    };
  },
  created() {
    if (this.enableWizard) {
      this.clearStatusesOnDisconnect();
    }
  },
  computed: {
    serialPorts() {
      return this.$store.getters.axtec.serialPorts;
    },
    isConnected() {
      return this.$store.getters.axtec.project.earthStation.connected;
    },
    port() {
      return this.$store.getters.axtec.project.earthStation.port;
    }
  },
  watch: {
    serialPorts(ports) {
      if (ports.length == 0) {
        debugger;
        this.USBPort = "";
        this.clearStatusesOnDisconnect();
        this.disconnect();
      }
    },
    isConnected(val) {
      if (!val) {
        this.USBPort = "";
        this.clearStatusesOnDisconnect();
        this.disconnect();
      } else {
        this.setStatusesOnConnect();
      }
    }
  },
  methods: {
    setStatusesOnConnect() {
      this.$store.commit("setPortEarthStation", this.USBPort);
      this.$store.commit("setStatusEarthStation", true);
    },
    clearStatusesOnDisconnect() {
      this.$store.commit("setPortEarthStation", "");
      this.$store.commit("setStatusEarthStation", false);
      this.$store.commit("setStatusCanSat", { index: 0, connected: false });
      this.$store.commit("setIDCanSat", { index: 0, id: "" });
      //this.$store.commit('setNameCanSat', { 'index': 0, 'name': ''})
    },
    connect() {
      if (this.USBPort != "") {
        CanSatAPI.connectToPort(this.USBPort);
      }
    },
    disconnect() {
      this.clearStatusesOnDisconnect();
      CanSatAPI.disconnectToPort();
    },
    isETConnected() {
      return this.$store.getters.axtec.project.earthStation.connected;
    },
    isValid() {
      return this.valid;
    }
  }
};
</script>

