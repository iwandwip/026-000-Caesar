#include <stdio.h>

void sendDowntimeText(const char* format, const char* value) {
  char command[96];
  snprintf(command, sizeof(command), format, value);
  sendCommand(command);
}

void showDowntimeInfo(const char* pageName) {
  sendCommand("page pageDashboard");
  delay(100);
  lockInterlock();
  sendDowntimeText("page %s", pageName);
}

void lockInterlock() {
  setInterlock(false);
  sendCommand("pageSys.nMReady.val=0");
  sendCommand("tILock.bco=63488");
  sendCommand("tILock.txt=\"\"");
}

void startFrontDowntime(const char* reason) {
  sendCommand("pageSys.nFDtAct.val=1");
  sendDowntimeText("pageSys.tFDtType.txt=\"%s\"", reason);
  sendCommand("pageSys.tFDtStart.txt=pageSys.tNow.txt");
  lockInterlock();
  updateDashboardStatus();
  showDowntimeInfo("pageDtInfoF");
}

void startBackDowntime(const char* reason) {
  sendCommand("pageSys.nBDtAct.val=1");
  sendDowntimeText("pageSys.tBDtType.txt=\"%s\"", reason);
  sendCommand("pageSys.tBDtStart.txt=pageSys.tNow.txt");
  lockInterlock();
  updateDashboardStatus();
  showDowntimeInfo("pageDtInfoB");
}

void startMachineDowntime(const char* reason) {
  sendCommand("pageSys.nMDtAct.val=1");
  sendDowntimeText("pageSys.tMDtType.txt=\"%s\"", reason);
  sendCommand("pageSys.tMDtStart.txt=pageSys.tNow.txt");
  lockInterlock();
  updateDashboardStatus();
  showDowntimeInfo("pageDtMcInfo");
}

void clearFrontDowntime() {
  sendCommand("pageSys.nFDtAct.val=0");
  sendCommand("pageSys.tFDtType.txt=\"\"");
  sendCommand("pageSys.tFDtStart.txt=\"\"");
  sendCommand("pageSys.nDtPop.val=0");
  updateDashboardStatus();
  sendCommand("page pageDashboard");
}

void clearBackDowntime() {
  sendCommand("pageSys.nBDtAct.val=0");
  sendCommand("pageSys.tBDtType.txt=\"\"");
  sendCommand("pageSys.tBDtStart.txt=\"\"");
  sendCommand("pageSys.nDtPop.val=0");
  updateDashboardStatus();
  sendCommand("page pageDashboard");
}

void clearMachineDowntime() {
  sendCommand("pageSys.nMDtAct.val=0");
  sendCommand("pageSys.tMDtType.txt=\"\"");
  sendCommand("pageSys.tMDtStart.txt=\"\"");
  sendCommand("pageSys.nDtPop.val=0");
  updateDashboardStatus();
  sendCommand("page pageDashboard");
}

void bBackDtFCallback(void* ptr) {
  sendCommand("page pageDashboard");
}

void bBhnFCallback(void* ptr) {
  startFrontDowntime("BAHAN TELAT");
}

void bTggPnsFCallback(void* ptr) {
  startFrontDowntime("TUNGGU PANAS");
}

void bMoffFCallback(void* ptr) {
  startFrontDowntime("MOULD OFF");
}

void bLbhPnsFCallback(void* ptr) {
  startFrontDowntime("LEBIH PANAS");
}

void bKrgPnsFCallback(void* ptr) {
  startFrontDowntime("KURANG PANAS");
}

void bRunFCallback(void* ptr) {
  clearFrontDowntime();
}

void bTrialFCallback(void* ptr) {
  startFrontDowntime("TRIAL");
}

void bBackDtBCallback(void* ptr) {
  sendCommand("page pageDashboard");
}

void bBhnBCallback(void* ptr) {
  startBackDowntime("BAHAN TELAT");
}

void bTggPnsBCallback(void* ptr) {
  startBackDowntime("TUNGGU PANAS");
}

void bMoffBCallback(void* ptr) {
  startBackDowntime("MOULD OFF");
}

void bLbhPnsBCallback(void* ptr) {
  startBackDowntime("LEBIH PANAS");
}

void bKrgPnsBCallback(void* ptr) {
  startBackDowntime("KURANG PANAS");
}

void bRunBCallback(void* ptr) {
  clearBackDowntime();
}

void bTrialBCallback(void* ptr) {
  startBackDowntime("TRIAL");
}

void bBackMcCallback(void* ptr) {
  sendCommand("page pageDashboard");
}

void bGMldMcCallback(void* ptr) {
  startMachineDowntime("GANTI MOULD");
}

void bCMldMcCallback(void* ptr) {
  startMachineDowntime("CLEANING MOULD");
}

void bTrMcCallback(void* ptr) {
  startMachineDowntime("TROUBLE MC");
}

void bTrTlMcCallback(void* ptr) {
  startMachineDowntime("TROUBLE TL");
}

void bSetMcCallback(void* ptr) {
  startMachineDowntime("SETTING MC");
}

void bPrevMcCallback(void* ptr) {
  startMachineDowntime("PREVENTIVE MC");
}

void bOffMcCallback(void* ptr) {
  startMachineDowntime("MC OFF");
}

void bRunMcCallback(void* ptr) {
  clearMachineDowntime();
}

void bOrisolMcCallback(void* ptr) {
  startMachineDowntime("ORISOL MAMPET");
}

void registerDowntimeCallbacks() {
  bBackDtF.attachPop(bBackDtFCallback);
  bBhnF.attachPop(bBhnFCallback);
  bTggPnsF.attachPop(bTggPnsFCallback);
  bMoffF.attachPop(bMoffFCallback);
  bLbhPnsF.attachPop(bLbhPnsFCallback);
  bKrgPnsF.attachPop(bKrgPnsFCallback);
  bRunF.attachPop(bRunFCallback);
  bTrialF.attachPop(bTrialFCallback);

  bBackDtB.attachPop(bBackDtBCallback);
  bBhnB.attachPop(bBhnBCallback);
  bTggPnsB.attachPop(bTggPnsBCallback);
  bMoffB.attachPop(bMoffBCallback);
  bLbhPnsB.attachPop(bLbhPnsBCallback);
  bKrgPnsB.attachPop(bKrgPnsBCallback);
  bRunB.attachPop(bRunBCallback);
  bTrialB.attachPop(bTrialBCallback);

  bBackMc.attachPop(bBackMcCallback);
  bGMldMc.attachPop(bGMldMcCallback);
  bCMldMc.attachPop(bCMldMcCallback);
  bTrMc.attachPop(bTrMcCallback);
  bTrTlMc.attachPop(bTrTlMcCallback);
  bSetMc.attachPop(bSetMcCallback);
  bPrevMc.attachPop(bPrevMcCallback);
  bOffMc.attachPop(bOffMcCallback);
  bRunMc.attachPop(bRunMcCallback);
  bOrisolMc.attachPop(bOrisolMcCallback);
}
