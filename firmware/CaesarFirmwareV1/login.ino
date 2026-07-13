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
  currentPageId = 0;
  sendCommand("pageSys.frontLogin.val=0");
  sendCommand("pageSys.nFOpId.val=0");
  sendCommand("pageSys.tFOpName.txt=\"\"");
  nIdF.setValue(0);
  tNameF.setText("");
  sendCommand("page pageDashboard");
}

void clearBackLogin() {
  currentPageId = 0;
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
  const Operator* op = findOperator(id);

  if (op == nullptr) {
    tNameF.setText("ID SALAH");
    return;
  }

  setFrontLogin(id, op->name);
}

void handleBackLogin() {
  uint32_t id = 0;
  nIdB.getValue(&id);
  const Operator* op = findOperator(id);

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

void pageCallback(void* ptr) {
  currentPageId = *((const uint8_t*)ptr);
}

void bBackFCallback(void* ptr) {
  currentPageId = 0;
}

void bBackBCallback(void* ptr) {
  currentPageId = 0;
}

void registerLoginCallbacks() {
  pageDashboard.attachPop(pageCallback, (void*)&PAGE_DASHBOARD_ID);
  pageLoginF.attachPop(pageCallback, (void*)&PAGE_LOGIN_F_ID);
  pageLoginB.attachPop(pageCallback, (void*)&PAGE_LOGIN_B_ID);
  pageMldF.attachPop(pageCallback, (void*)&PAGE_MLD_F_ID);
  pageMldB.attachPop(pageCallback, (void*)&PAGE_MLD_B_ID);
  pageLotF.attachPop(pageCallback, (void*)&PAGE_LOT_F_ID);
  pageLotB.attachPop(pageCallback, (void*)&PAGE_LOT_B_ID);
  pageOutputNG.attachPop(pageCallback, (void*)&PAGE_OUTPUT_NG_ID);
  pageInputNGF.attachPop(pageCallback, (void*)&PAGE_INPUT_NG_F_ID);
  pageInputNGB.attachPop(pageCallback, (void*)&PAGE_INPUT_NG_B_ID);
  pageDtF.attachPop(pageCallback, (void*)&PAGE_DT_F_ID);
  pageDtB.attachPop(pageCallback, (void*)&PAGE_DT_B_ID);
  pageDtInfoF.attachPop(pageCallback, (void*)&PAGE_DT_INFO_F_ID);
  pageDtInfoB.attachPop(pageCallback, (void*)&PAGE_DT_INFO_B_ID);
  pageDtMc.attachPop(pageCallback, (void*)&PAGE_DT_MC_ID);
  pageDtMcInfo.attachPop(pageCallback, (void*)&PAGE_DT_MC_INFO_ID);
  pageSim.attachPop(pageCallback, (void*)&PAGE_SIM_ID);
  pageSys.attachPop(pageCallback, (void*)&PAGE_SYS_ID);
  keybdA.attachPop(pageCallback, (void*)&PAGE_KEYBD_A_ID);
  keybdB.attachPop(pageCallback, (void*)&PAGE_KEYBD_B_ID);
  bOkF.attachPop(bOkFCallback);
  bOkB.attachPop(bOkBCallback);
  bLogoutF.attachPop(bLogoutFCallback);
  bLogoutB.attachPop(bLogoutBCallback);
  bBackF.attachPop(bBackFCallback);
  bBackB.attachPop(bBackBCallback);
}
