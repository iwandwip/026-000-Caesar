#include <stdio.h>

void sendInputText(const char* component, const char* value) {
  char command[96];
  snprintf(command, sizeof(command), "%s.txt=\"%s\"", component, value);
  sendCommand(command);
}

void sendInputValue(const char* component, uint32_t value) {
  char command[64];
  snprintf(command, sizeof(command), "%s.val=%lu", component, (unsigned long)value);
  sendCommand(command);
}

void fillFrontMouldInput(const char* code) {
  tInMF.setText(code);
}

void fillBackMouldInput(const char* code) {
  tInMB.setText(code);
}

void fillFrontLotInput(const char* code) {
  tInLotF.setText(code);

  uint32_t cavity = 0;
  nFCavSys.getValue(&cavity);
  nIsiLotF.setValue(cavity);
}

void fillBackLotInput(const char* code) {
  tInLotB.setText(code);

  uint32_t cavity = 0;
  nBCavSys.getValue(&cavity);
  nIsiLotB.setValue(cavity);
}

void handleFrontMould() {
  char code[32];
  tInMF.getText(code, sizeof(code));
  const Mould* mould = findMould(code);

  if (mould == nullptr) {
    sendInputText("tModMF", "MOULD NG");
    sendInputValue("nCavF", 0);
    sendInputValue("nIsiF", 0);
    return;
  }

  sendInputText("tModMF", mould->model);
  sendInputValue("nCavF", mould->cavity);
  sendInputValue("nIsiF", mould->cavity);
  sendInputText("pageSys.tFMld", code);
  sendInputText("pageSys.tFMldModel", mould->model);
  sendInputValue("pageSys.nFCav", mould->cavity);
  sendInputValue("pageSys.nFIsi", mould->cavity);
}

void handleBackMould() {
  char code[32];
  tInMB.getText(code, sizeof(code));
  const Mould* mould = findMould(code);

  if (mould == nullptr) {
    sendInputText("tModMB", "MOULD NG");
    sendInputValue("nCavB", 0);
    sendInputValue("nIsiB", 0);
    return;
  }

  sendInputText("tModMB", mould->model);
  sendInputValue("nCavB", mould->cavity);
  sendInputValue("nIsiB", mould->cavity);
  sendInputText("pageSys.tBMld", code);
  sendInputText("pageSys.tBMldModel", mould->model);
  sendInputValue("pageSys.nBCav", mould->cavity);
  sendInputValue("pageSys.nBIsi", mould->cavity);
}

void handleFrontLot() {
  char code[32];
  tInLotF.getText(code, sizeof(code));
  const Lot* lot = findLot(code);

  if (lot == nullptr) {
    sendInputText("tModF", "LOT NG");
    sendInputValue("nTgtF", 0);
    sendInputValue("nQuoF", 0);
    return;
  }

  sendInputText("tModF", lot->model);
  sendInputValue("nTgtF", lot->target);
  sendInputValue("nQuoF", lot->target);

  uint32_t cavity = 0;
  uint32_t isi = 0;
  nFCavSys.getValue(&cavity);
  nIsiLotF.getValue(&isi);

  if (cavity == 0) {
    sendInputText("tModF", "NO MOULD");
    return;
  }

  if (isi == 0) {
    sendInputText("tModF", "ISI 0");
    return;
  }

  if (isi > cavity) {
    sendInputText("tModF", "ISI > CAV");
    return;
  }

  sendInputText("pageSys.tFLot", code);
  sendInputText("pageSys.tFModel", lot->model);
  sendInputValue("pageSys.nFTarget", lot->target);
  sendInputValue("pageSys.nFQuota", lot->target);
  sendInputValue("pageSys.nFIsi", isi);
}

void handleBackLot() {
  char code[32];
  tInLotB.getText(code, sizeof(code));
  const Lot* lot = findLot(code);

  if (lot == nullptr) {
    sendInputText("tModB", "LOT NG");
    sendInputValue("nTgtB", 0);
    sendInputValue("nQuoB", 0);
    return;
  }

  sendInputText("tModB", lot->model);
  sendInputValue("nTgtB", lot->target);
  sendInputValue("nQuoB", lot->target);

  uint32_t cavity = 0;
  uint32_t isi = 0;
  nBCavSys.getValue(&cavity);
  nIsiLotB.getValue(&isi);

  if (cavity == 0) {
    sendInputText("tModB", "NO MOULD");
    return;
  }

  if (isi == 0) {
    sendInputText("tModB", "ISI 0");
    return;
  }

  if (isi > cavity) {
    sendInputText("tModB", "ISI > CAV");
    return;
  }

  sendInputText("pageSys.tBLot", code);
  sendInputText("pageSys.tBModel", lot->model);
  sendInputValue("pageSys.nBTarget", lot->target);
  sendInputValue("pageSys.nBQuota", lot->target);
  sendInputValue("pageSys.nBIsi", isi);
}

void clearFrontMould() {
  tInMF.setText("");
  sendInputText("tModMF", "");
  sendInputValue("nCavF", 0);
  sendInputValue("nIsiF", 0);
  sendInputText("pageSys.tFMld", "");
  sendInputText("pageSys.tFMldModel", "");
  sendInputValue("pageSys.nFCav", 0);
  sendInputValue("pageSys.nFIsi", 0);
}

void clearBackMould() {
  tInMB.setText("");
  sendInputText("tModMB", "");
  sendInputValue("nCavB", 0);
  sendInputValue("nIsiB", 0);
  sendInputText("pageSys.tBMld", "");
  sendInputText("pageSys.tBMldModel", "");
  sendInputValue("pageSys.nBCav", 0);
  sendInputValue("pageSys.nBIsi", 0);
}

void clearFrontLot() {
  uint32_t cavity = 0;
  nFCavSys.getValue(&cavity);
  tInLotF.setText("");
  sendInputText("tModF", "");
  sendInputValue("nTgtF", 0);
  sendInputValue("nQuoF", 0);
  nIsiLotF.setValue(cavity);
  sendInputText("pageSys.tFLot", "");
  sendInputText("pageSys.tFModel", "");
  sendInputValue("pageSys.nFTarget", 0);
  sendInputValue("pageSys.nFQuota", 0);
  sendInputValue("pageSys.nFIsi", cavity);
}

void clearBackLot() {
  uint32_t cavity = 0;
  nBCavSys.getValue(&cavity);
  tInLotB.setText("");
  sendInputText("tModB", "");
  sendInputValue("nTgtB", 0);
  sendInputValue("nQuoB", 0);
  nIsiLotB.setValue(cavity);
  sendInputText("pageSys.tBLot", "");
  sendInputText("pageSys.tBModel", "");
  sendInputValue("pageSys.nBTarget", 0);
  sendInputValue("pageSys.nBQuota", 0);
  sendInputValue("pageSys.nBIsi", cavity);
}

void bOkMFCallback(void* ptr) {
  handleFrontMould();
}

void bOkMBCallback(void* ptr) {
  handleBackMould();
}

void bOkLotFCallback(void* ptr) {
  handleFrontLot();
}

void bOkLotBCallback(void* ptr) {
  handleBackLot();
}

void bClearMFCallback(void* ptr) {
  clearFrontMould();
}

void bClearMBCallback(void* ptr) {
  clearBackMould();
}

void bClearLotFCallback(void* ptr) {
  clearFrontLot();
}

void bClearLotBCallback(void* ptr) {
  clearBackLot();
}

void registerInputCallbacks() {
  bOkMF.attachPop(bOkMFCallback);
  bOkMB.attachPop(bOkMBCallback);
  bOkLotF.attachPop(bOkLotFCallback);
  bOkLotB.attachPop(bOkLotBCallback);
  bClearMF.attachPop(bClearMFCallback);
  bClearMB.attachPop(bClearMBCallback);
  bClearLotF.attachPop(bClearLotFCallback);
  bClearLotB.attachPop(bClearLotBCallback);
}
