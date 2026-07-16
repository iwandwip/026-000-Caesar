const LayerComponents FRONT_COMPONENTS = {
  "pageSys.nFCyc", "pageSys.nFOut", "pageSys.nFOK", "pageSys.nFNG",
  "pageSys.nFQuota", "pageSys.nFIsi", "pageSys.nFTarget",
  "pageSys.tFModel", "pageSys.tFLot", "pageSys.tFOpName"
};

const LayerComponents BACK_COMPONENTS = {
  "pageSys.nBCyc", "pageSys.nBOut", "pageSys.nBOK", "pageSys.nBNG",
  "pageSys.nBQuota", "pageSys.nBIsi", "pageSys.nBTarget",
  "pageSys.tBModel", "pageSys.tBLot", "pageSys.tBOpName"
};

bool readLayerState(const LayerComponents& components, LayerState* state) {
  memset(state, 0, sizeof(*state));

  if (!readNextionValue(components.cycle, &state->cycle) || !readNextionValue(components.output, &state->output) || !readNextionValue(components.ok, &state->ok) || !readNextionValue(components.ng, &state->ng) || !readNextionValue(components.quota, &state->quota) || !readNextionValue(components.isi, &state->isi) || !readNextionValue(components.target, &state->target)) {
    return false;
  }

  readNextionText(components.model, state->model, sizeof(state->model));
  readNextionText(components.lot, state->lot, sizeof(state->lot));
  readNextionText(components.operatorName, state->operatorName, sizeof(state->operatorName));
  return true;
}

void publishLayerState(const char* topic, const LayerComponents& components) {
  if (!mqttClient.connected()) {
    return;
  }

  LayerState state;
  if (!readLayerState(components, &state)) {
    return;
  }

  JsonDocument doc;
  doc["cycle"] = state.cycle;
  doc["output"] = state.output;
  doc["ok"] = state.ok;
  doc["ng"] = state.ng;
  doc["quota"] = state.quota;
  doc["isi"] = state.isi;
  doc["target"] = state.target;
  doc["model"] = state.model;
  doc["lot"] = state.lot;
  doc["operator"] = state.operatorName;
  doc["startTime"] = cycleStartTime;
  doc["finishTime"] = cycleFinishTime;

  // PubSubClient buffer is configured to 512 bytes in setupMqtt().
  char payload[384];
  size_t length = serializeJson(doc, payload, sizeof(payload));
  if (length > 0 && length < sizeof(payload)) {
    mqttClient.publish(topic, payload, true);
  }
}

void publishBothLayers() {
  publishLayerState(DATA_A_TOPIC, FRONT_COMPONENTS);
  publishLayerState(DATA_B_TOPIC, BACK_COMPONENTS);
}
