#include "config.h"
#include "Nextion.h"

const uint8_t PAGE_LOGIN_F_ID = 1;
const uint8_t PAGE_LOGIN_B_ID = 2;

NexPage pageLoginF = NexPage(PAGE_LOGIN_F_ID, 0, "pageLoginF");
NexButton bOkF = NexButton(PAGE_LOGIN_F_ID, 2, "bOkF");
NexButton bLogoutF = NexButton(PAGE_LOGIN_F_ID, 3, "bLogoutF");
NexButton bBackF = NexButton(PAGE_LOGIN_F_ID, 4, "bBackF");
NexText tNameF = NexText(PAGE_LOGIN_F_ID, 7, "tNameF");
NexNumber nIdF = NexNumber(PAGE_LOGIN_F_ID, 9, "nIdF");

NexPage pageLoginB = NexPage(PAGE_LOGIN_B_ID, 0, "pageLoginB");
NexButton bOkB = NexButton(PAGE_LOGIN_B_ID, 2, "bOkB");
NexButton bLogoutB = NexButton(PAGE_LOGIN_B_ID, 3, "bLogoutB");
NexButton bBackB = NexButton(PAGE_LOGIN_B_ID, 4, "bBackB");
NexText tNameB = NexText(PAGE_LOGIN_B_ID, 7, "tNameB");
NexNumber nIdB = NexNumber(PAGE_LOGIN_B_ID, 9, "nIdB");

NexTouch* nex_listen_list[] = {
    &pageLoginF, &bOkF, &bLogoutF, &bBackF, &tNameF, &nIdF,
    &pageLoginB, &bOkB, &bLogoutB, &bBackB, &tNameB, &nIdB,
    NULL
};

void registerLoginCallbacks();

void setup() {
    Serial.begin(115200);
#if NEXTION_MODE_DEVICE
    Serial2.begin(115200);
#endif
    registerLoginCallbacks();
}

void loop() {
    nexLoop(nex_listen_list);
}
