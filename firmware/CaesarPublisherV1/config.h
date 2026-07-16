#ifndef CAESAR_PUBLISHER_CONFIG_H
#define CAESAR_PUBLISHER_CONFIG_H

#define WIFI_SSID "TIMEO"
#define WIFI_PASS "1234Saja"
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883

#define MQTT_ADDR "91c21b8614cd"
#define MQTT_CLIENT_ID "CaesarPublisher/" MQTT_ADDR
#define CONTROL_TOPIC "CONTROL/IOTHP-BP/" MQTT_ADDR
#define START_TOPIC "start/IOTHP-BP/" MQTT_ADDR
#define FINISH_TOPIC "finish/IOTHP-BP/" MQTT_ADDR

#define NTP_SERVER "pool.ntp.org"
#define WIB_TIMEZONE "WIB-7"

#define FRONT_PIN 32
#define BACK_PIN 35
#define UP_PIN 33
#define DOWN_PIN 25
#define WIFI_LED_PIN 26

#define OTA_USERNAME "admin"
#define OTA_PASSWORD "123654789"

#endif
