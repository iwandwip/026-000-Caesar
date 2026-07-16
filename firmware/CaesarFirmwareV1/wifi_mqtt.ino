unsigned long lastWifiAttempt = 0;
unsigned long lastMqttAttempt = 0;

void mqttCallback(char* topic, uint8_t* payload, unsigned int length) {
  char message[32];
  unsigned int messageLength = length < sizeof(message) - 1 ? length : sizeof(message) - 1;
  memcpy(message, payload, messageLength);
  message[messageLength] = '\0';

  if (strcmp(topic, CONTROL_TOPIC) == 0 && strcmp(message, "REBOOT") == 0) {
    ESP.restart();
    return;
  }

  if (strcmp(topic, FINISH_TOPIC) == 0 && strcmp(message, "1cycle") == 0) {
    handleMqttCycle();
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
