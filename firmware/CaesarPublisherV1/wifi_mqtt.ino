unsigned long lastWifiAttempt = 0;
unsigned long lastMqttAttempt = 0;

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, CONTROL_TOPIC) == 0 && length == 6 && memcmp(payload, "REBOOT", 6) == 0) {
    ESP.restart();
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
  mqttClient.setBufferSize(256);
  mqttClient.setKeepAlive(60);
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
  }
}

void mqttLoop() {
  reconnectWifi();
  digitalWrite(WIFI_LED_PIN, WiFi.status() == WL_CONNECTED ? HIGH : LOW);
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  reconnectMqtt();
  if (mqttClient.connected()) {
    mqttClient.loop();
  }
}
