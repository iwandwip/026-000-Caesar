#include <ESP32HTTPUpdateServer.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <WiFi.h>
#include <time.h>

#include "config.h"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
WebServer httpServer(80);
ESP32HTTPUpdateServer httpUpdater;

void setup() {
  Serial.begin(115200);

  pinMode(FRONT_PIN, INPUT);
  pinMode(BACK_PIN, INPUT);
  pinMode(UP_PIN, INPUT);
  pinMode(DOWN_PIN, INPUT);
  pinMode(WIFI_LED_PIN, OUTPUT);
  digitalWrite(WIFI_LED_PIN, LOW);

  initCycleInputs();
  initTime();
  setupWifi();
  setupMqtt();

  httpUpdater.setup(&httpServer, "/update", OTA_USERNAME, OTA_PASSWORD);
  httpServer.begin();
}

void loop() {
  mqttLoop();
  syncTime();
  httpServer.handleClient();
  updateCycleInputs();
}
