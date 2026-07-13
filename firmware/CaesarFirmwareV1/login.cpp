#include "login.h"
#include "config.h"
#include <stdio.h>

const uint8_t PAGE_LOGIN_F_ID = 2;
const uint8_t PAGE_LOGIN_B_ID = 3;

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

void sendFormattedCommand(const char* format, const char* value) {
    char command[96];
    snprintf(command, sizeof(command), format, value);
    sendCommand(command);
}

void sendFormattedCommand(const char* format, uint32_t value) {
    char command[64];
    snprintf(command, sizeof(command), format, (unsigned long)value);
    sendCommand(command);
}

void setFrontLogin(uint32_t id, const char* name) {
    sendCommand("pageSys.frontLogin.val=1");
    sendFormattedCommand("pageSys.nFOpId.val=%lu", id);
    sendFormattedCommand("pageSys.tFOpName.txt=\"%s\"", name);
    tNameF.setText(name);
}

void setBackLogin(uint32_t id, const char* name) {
    sendCommand("pageSys.backLogin.val=1");
    sendFormattedCommand("pageSys.nBOpId.val=%lu", id);
    sendFormattedCommand("pageSys.tBOpName.txt=\"%s\"", name);
    tNameB.setText(name);
}

void clearFrontLogin() {
    sendCommand("pageSys.frontLogin.val=0");
    sendCommand("pageSys.nFOpId.val=0");
    sendCommand("pageSys.tFOpName.txt=\"\"");
    nIdF.setValue(0);
    tNameF.setText("");
    sendCommand("page pageDashboard");
}

void clearBackLogin() {
    sendCommand("pageSys.backLogin.val=0");
    sendCommand("pageSys.nBOpId.val=0");
    sendCommand("pageSys.tBOpName.txt=\"\"");
    nIdB.setValue(0);
    tNameB.setText("");
    sendCommand("page pageDashboard");
}

void handleLogin(NexNumber& input, NexText& display, void (*setLogin)(uint32_t, const char*)) {
    uint32_t id = 0;
    input.getValue(&id);
    const Operator* op = findOperator((uint16_t)id);
    if (op == nullptr) {
        display.setText("ID SALAH");
        return;
    }
    setLogin(id, op->name);
}

void bOkFCallback(void* ptr) {
    handleLogin(nIdF, tNameF, setFrontLogin);
}

void bOkBCallback(void* ptr) {
    handleLogin(nIdB, tNameB, setBackLogin);
}

void bLogoutFCallback(void* ptr) {
    clearFrontLogin();
}

void bLogoutBCallback(void* ptr) {
    clearBackLogin();
}

void registerLoginCallbacks() {
    bOkF.attachPop(bOkFCallback);
    bOkB.attachPop(bOkBCallback);
    bLogoutF.attachPop(bLogoutFCallback);
    bLogoutB.attachPop(bLogoutBCallback);
}
