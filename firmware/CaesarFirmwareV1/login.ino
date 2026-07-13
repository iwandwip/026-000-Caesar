#include <stdio.h>

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

void handleFrontLogin() {
    uint32_t id = 0;
    nIdF.getValue(&id);
    const Operator* op = findOperator((uint16_t)id);

    if (op == nullptr) {
        tNameF.setText("ID SALAH");
        return;
    }

    setFrontLogin(id, op->name);
}

void handleBackLogin() {
    uint32_t id = 0;
    nIdB.getValue(&id);
    const Operator* op = findOperator((uint16_t)id);

    if (op == nullptr) {
        tNameB.setText("ID SALAH");
        return;
    }

    setBackLogin(id, op->name);
}

void bOkFCallback(void* ptr) {
    handleFrontLogin();
}

void bOkBCallback(void* ptr) {
    handleBackLogin();
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
