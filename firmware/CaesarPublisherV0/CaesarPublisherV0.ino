#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP32HTTPUpdateServer.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPUpdateServer.h>

WiFiClient espClient;
PubSubClient client(espClient);


const int AddressPin[6] = {5, 17, 16, 4, 27, 15};

int FrontPin = 32;
int BackPin  = 35;
int UpPin    = 33;
int DownPin  = 25;

const int wfledPin = 26;


bool frontSent = false;
bool backSent  = false;
bool upSent    = false;
bool downSent  = false;
bool upAllowed = true;


bool cycleStarted = false;
int stepCount = 0;
bool expectUp = true;


const char* ssid        = "Ruckus_Barcode_Plant2";
const char* password    = "indoprima";
const char* mqtt_server = "192.168.10.10";
const int   mqtt_port   = 1883;

String full_mqtt_topic;
String control_mqtt_topic;
String read_mqtt_topic;
String wifi_mqtt_topic;
String start_mqtt_topic;
String finish_mqtt_topic;
String ip_mqtt_topic;
String esp_client;


WebServer httpServer(80);
ESP32HTTPUpdateServer httpUpdater;
const char* ota_username = "admin";
const char* ota_password = "123654789";


void setup_wifi(){
  WiFi.begin(ssid, password);
  WiFi.setHostname(full_mqtt_topic.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());

  httpUpdater.setup(&httpServer, "/update", ota_username, ota_password);
  httpServer.begin();
}


void reconnect() {
  while (!client.connected()) {
    if (client.connect(esp_client.c_str())) {
      client.subscribe(control_mqtt_topic.c_str());
    } else {
      delay(2000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  if (msg == "REBOOT") ESP.restart();
}


void Address(){
  int address = 0;

  for (int i = 0; i < 6; i++) {
    pinMode(AddressPin[i], INPUT_PULLUP);
    bitWrite(address, i, digitalRead(AddressPin[i]));
  }

  full_mqtt_topic = "IOTHP-BP/" + String(address);

  esp_client = "CLIENT/" + full_mqtt_topic;
  control_mqtt_topic = "CONTROL/" + full_mqtt_topic;
  read_mqtt_topic    = "READ/" + full_mqtt_topic;
  wifi_mqtt_topic    = "WIFI/" + full_mqtt_topic;
  ip_mqtt_topic      = "IP/" + full_mqtt_topic;
  start_mqtt_topic   = "start/" + full_mqtt_topic;
  finish_mqtt_topic  = "finish/" + full_mqtt_topic;
}


void setup(){
  Serial.begin(115200);

  pinMode(FrontPin, INPUT);
  pinMode(BackPin,  INPUT);
  pinMode(UpPin,    INPUT);
  pinMode(DownPin,  INPUT);
  pinMode(wfledPin, OUTPUT);

  Address();
  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

/* ================= LOOP ================= */
void loop() {

  client.loop();
  httpServer.handleClient();

  if (!client.connected()) reconnect();

  bool front = digitalRead(FrontPin);
  bool back  = digitalRead(BackPin);
  bool up    = digitalRead(UpPin);
  bool down  = digitalRead(DownPin);

  
  if (front == LOW && !frontSent) {

    if (!cycleStarted) {
      client.publish(start_mqtt_topic.c_str(), "START");
      cycleStarted = true;
      stepCount = 0;
      expectUp = true;
    }

    frontSent = true;
  }
  if (front == HIGH) frontSent = false;

  
  if (back == LOW && !backSent) {

    if (!cycleStarted) {
      client.publish(start_mqtt_topic.c_str(), "START");
      cycleStarted = true;
      stepCount = 0;
      expectUp = true;
    }

    backSent = true;
  }
  if (back == HIGH) backSent = false;

 
  if (up == LOW && !upSent && upAllowed) {

    if (cycleStarted && expectUp) {
      stepCount++;
      expectUp = false;
    }

    upAllowed = false;
    upSent = true;
  }
  if (up == HIGH) upSent = false;

  
  if (down == LOW && !downSent) {

    if (cycleStarted && !expectUp) {
      stepCount++;
      expectUp = true;
    }

    upAllowed = true;
    downSent = true;
  }
  if (down == HIGH) downSent = false;

 
  if (cycleStarted && stepCount >= 5) {

    client.publish(finish_mqtt_topic.c_str(), "1cycle");

    Serial.println("===== 1 CYCLE COMPLETE =====");

    cycleStarted = false;
    stepCount = 0;
  }
}