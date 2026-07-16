#ifndef CAESAR_CONFIG_H
#define CAESAR_CONFIG_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <Wire.h>
#include <RTClib.h>

#define SCANNER_SERIAL Serial2
#define SCANNER_RX_PIN 16
#define SCANNER_TX_PIN 17
#define SCANNER_BAUD 9600

#define WIFI_SSID "S22 5"
#define WIFI_PASS "55555555"
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_ADDR "be57077be216"
#define MQTT_CLIENT_ID "IOTHP-BP/" MQTT_ADDR
#define CONTROL_TOPIC "CONTROL/IOTHP-BP/" MQTT_ADDR
#define FINISH_TOPIC "finish/IOTHP-BP/" MQTT_ADDR
#define DATA_A_TOPIC "dataA/IOTHP-BP/" MQTT_ADDR
#define DATA_B_TOPIC "dataB/IOTHP-BP/" MQTT_ADDR

struct Operator {
  uint32_t id;
  const char* name;
};

static const Operator OPERATORS[] = {
  { 1111, "Budi Andianto" },
  { 2222, "Andi Saputra" },
  { 3333, "Siti Nurhaliza" }
};

static const size_t OPERATOR_COUNT = sizeof(OPERATORS) / sizeof(OPERATORS[0]);

inline const Operator* findOperator(uint32_t id) {
  for (size_t i = 0; i < OPERATOR_COUNT; i++) {
    if (OPERATORS[i].id == id) {
      return &OPERATORS[i];
    }
  }
  return nullptr;
}

struct Mould {
  const char* code;
  const char* model;
  uint8_t cavity;
};

static const Mould MOULDS[] = {
  { "1111", "MOULD A", 8 },
  { "2222", "MOULD B", 6 },
  { "3333", "MOULD C", 4 }
};

static const size_t MOULD_COUNT = sizeof(MOULDS) / sizeof(MOULDS[0]);

inline const Mould* findMould(const char* code) {
  for (size_t i = 0; i < MOULD_COUNT; i++) {
    if (strcmp(MOULDS[i].code, code) == 0) {
      return &MOULDS[i];
    }
  }
  return nullptr;
}

struct Lot {
  const char* code;
  const char* model;
  uint16_t target;
};

static const Lot LOTS[] = {
  { "1111", "DEP FUSO", 108 },
  { "2222", "VARIO125", 120 },
  { "3333", "GENIO", 96 }
};

static const size_t LOT_COUNT = sizeof(LOTS) / sizeof(LOTS[0]);

inline const Lot* findLot(const char* code) {
  for (size_t i = 0; i < LOT_COUNT; i++) {
    if (strcmp(LOTS[i].code, code) == 0) {
      return &LOTS[i];
    }
  }
  return nullptr;
}

#endif
