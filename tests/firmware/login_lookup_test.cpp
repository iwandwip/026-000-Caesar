#include <cassert>
#include <cstring>
#include "../../firmware/CaesarFirmwareV1/config.h"

int main() {
    const Operator* budi = findOperator(1111);
    assert(budi != nullptr);
    assert(budi->id == 1111);
    assert(std::strcmp(budi->name, "BUDI") == 0);

    const Operator* missing = findOperator(9999);
    assert(missing == nullptr);
}
