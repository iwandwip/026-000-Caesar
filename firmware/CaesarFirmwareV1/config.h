#ifndef CAESAR_CONFIG_H
#define CAESAR_CONFIG_H

#include <stddef.h>
#include <stdint.h>
#include <Wire.h>
#include <RTClib.h>

#define SCANNER_SERIAL Serial2
#define SCANNER_RX_PIN 16
#define SCANNER_TX_PIN 17
#define SCANNER_BAUD 9600

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

#endif
