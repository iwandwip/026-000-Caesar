#include "Nextion.h"
#include "login.h"

void setup() {
    Serial.begin(115200);
    Serial2.begin(115200);
    registerLoginCallbacks();
}

void loop() {
    nexLoop(nex_listen_list);
}
