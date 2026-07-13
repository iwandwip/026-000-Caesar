#ifndef CAESAR_CONFIG_H
#define CAESAR_CONFIG_H

#include <stddef.h>
#include <stdint.h>

struct Operator {
    uint16_t id;
    const char* name;
};

static const Operator OPERATORS[] = {
    {1111, "BUDI"},
    {2222, "ANDI"},
    {3333, "SITI"}
};

static const size_t OPERATOR_COUNT = sizeof(OPERATORS) / sizeof(OPERATORS[0]);

inline const Operator* findOperator(uint16_t id) {
    for (size_t i = 0; i < OPERATOR_COUNT; i++) {
        if (OPERATORS[i].id == id) {
            return &OPERATORS[i];
        }
    }
    return nullptr;
}

#endif
