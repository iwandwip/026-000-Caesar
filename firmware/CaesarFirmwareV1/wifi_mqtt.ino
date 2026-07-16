unsigned long lastWifiAttempt = 0;
unsigned long lastMqttAttempt = 0;

void publishEvent(const char* json) {
  if (!mqttClient.connected()) {
    return;
  }

  mqttClient.publish(EVENT_TOPIC, json, false);
}

void mqttCallback(char* topic, uint8_t* payload, unsigned int length) {
  if (strcmp(topic, CONTROL_TOPIC) == 0 && length == 6 && memcmp(payload, "REBOOT", 6) == 0) {
    ESP.restart();
    return;
  }

  if (strcmp(topic, FINISH_TOPIC) == 0) {
    JsonDocument doc;
    if (deserializeJson(doc, payload, length)) {
      return;
    }

    const char* event = doc["event"];
    const char* startTime = doc["startTime"];
    const char* finishTime = doc["finishTime"];
    if (event != nullptr && startTime != nullptr && finishTime != nullptr && strcmp(event, "1cycle") == 0) {
      handleMqttCycle(startTime, finishTime);
    }
  }
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(MQTT_CLIENT_ID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  lastWifiAttempt = millis();
}

void setupMqtt() {
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  mqttClient.setBufferSize(512);
  mqttClient.setKeepAlive(60);
  mqttClient.setSocketTimeout(5);
}

void reconnectWifi() {
  if (WiFi.status() == WL_CONNECTED || millis() - lastWifiAttempt < 5000) {
    return;
  }

  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  lastWifiAttempt = millis();
}

void reconnectMqtt() {
  if (mqttClient.connected() || (lastMqttAttempt != 0 && millis() - lastMqttAttempt < 5000)) {
    return;
  }

  lastMqttAttempt = millis();
  if (mqttClient.connect(MQTT_CLIENT_ID)) {
    mqttClient.subscribe(CONTROL_TOPIC);
    mqttClient.subscribe(FINISH_TOPIC);
  }
}

void mqttLoop() {
  reconnectWifi();
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  reconnectMqtt();
  if (mqttClient.connected()) {
    mqttClient.loop();
  }
}
