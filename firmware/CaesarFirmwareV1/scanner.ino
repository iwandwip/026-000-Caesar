#include <stdlib.h>
#include <stdio.h>

char scannerBuffer[32];
uint8_t scannerIdx = 0;

void sendScannerText(const char* component, const char* value) {
  char command[96];
  snprintf(command, sizeof(command), "%s.txt=\"%s\"", component, value);
  sendCommand(command);
}

void sendScannerValue(const char* component, uint32_t value) {
  char command[64];
  snprintf(command, sizeof(command), "%s.val=%lu", component, (unsigned long)value);
  sendCommand(command);
}

void processFrontMouldScan(const char* code) {
  const Mould* mould = findMould(code);
  sendScannerText("tInMF", code);

  if (mould == nullptr) {
    sendScannerText("tModMF", "MOULD NG");
    sendScannerValue("nCavF", 0);
    sendScannerValue("nIsiF", 0);
    return;
  }

  sendScannerText("tModMF", mould->model);
  sendScannerValue("nCavF", mould->cavity);
  sendScannerValue("nIsiF", mould->cavity);
  sendScannerText("pageSys.tFMld", code);
  sendScannerText("pageSys.tFMldModel", mould->model);
  sendScannerValue("pageSys.nFCav", mould->cavity);
  sendScannerValue("pageSys.nFIsi", mould->cavity);
}

void processBackMouldScan(const char* code) {
  const Mould* mould = findMould(code);
  sendScannerText("tInMB", code);

  if (mould == nullptr) {
    sendScannerText("tModMB", "MOULD NG");
    sendScannerValue("nCavB", 0);
    sendScannerValue("nIsiB", 0);
    return;
  }

  sendScannerText("tModMB", mould->model);
  sendScannerValue("nCavB", mould->cavity);
  sendScannerValue("nIsiB", mould->cavity);
  sendScannerText("pageSys.tBMld", code);
  sendScannerText("pageSys.tBMldModel", mould->model);
  sendScannerValue("pageSys.nBCav", mould->cavity);
  sendScannerValue("pageSys.nBIsi", mould->cavity);
}

void processFrontLotScan(const char* code) {
  const Lot* lot = findLot(code);
  sendScannerText("tInF", code);

  if (lot == nullptr) {
    sendScannerText("tModF", "LOT NG");
    sendScannerValue("nTgtF", 0);
    sendScannerValue("nQuoF", 0);
    return;
  }

  sendScannerText("tModF", lot->model);
  sendScannerValue("nTgtF", lot->target);
  sendScannerValue("nQuoF", lot->target);

  uint32_t cavity = 0;
  nFCavSys.getValue(&cavity);

  if (cavity == 0) {
    sendScannerText("tModF", "NO MOULD");
    sendScannerValue("nIsiLotF", 0);
    return;
  }

  sendScannerValue("nIsiLotF", cavity);
  sendScannerText("pageSys.tFLot", code);
  sendScannerText("pageSys.tFModel", lot->model);
  sendScannerValue("pageSys.nFTarget", lot->target);
  sendScannerValue("pageSys.nFQuota", lot->target);
  sendScannerValue("pageSys.nFIsi", cavity);
}

void processBackLotScan(const char* code) {
  const Lot* lot = findLot(code);
  sendScannerText("tInB", code);

  if (lot == nullptr) {
    sendScannerText("tModB", "LOT NG");
    sendScannerValue("nTgtB", 0);
    sendScannerValue("nQuoB", 0);
    return;
  }

  sendScannerText("tModB", lot->model);
  sendScannerValue("nTgtB", lot->target);
  sendScannerValue("nQuoB", lot->target);

  uint32_t cavity = 0;
  nBCavSys.getValue(&cavity);

  if (cavity == 0) {
    sendScannerText("tModB", "NO MOULD");
    sendScannerValue("nIsiLotB", 0);
    return;
  }

  sendScannerValue("nIsiLotB", cavity);
  sendScannerText("pageSys.tBLot", code);
  sendScannerText("pageSys.tBModel", lot->model);
  sendScannerValue("pageSys.nBTarget", lot->target);
  sendScannerValue("pageSys.nBQuota", lot->target);
  sendScannerValue("pageSys.nBIsi", cavity);
}

void processScannerInput(const char* input) {
  if (currentPageId == PAGE_MLD_F_ID) {
    processFrontMouldScan(input);
    return;
  }

  if (currentPageId == PAGE_MLD_B_ID) {
    processBackMouldScan(input);
    return;
  }

  if (currentPageId == PAGE_LOT_F_ID) {
    processFrontLotScan(input);
    return;
  }

  if (currentPageId == PAGE_LOT_B_ID) {
    processBackLotScan(input);
    return;
  }

  uint32_t id = strtoul(input, NULL, 10);
  const Operator* op = findOperator(id);

  if (currentPageId == PAGE_LOGIN_F_ID) {
    nIdF.setValue(id);

    if (op == nullptr) {
      tNameF.setText("ID SALAH");
      return;
    }

    setFrontLogin(id, op->name);
    return;
  }

  if (currentPageId == PAGE_LOGIN_B_ID) {
    nIdB.setValue(id);

    if (op == nullptr) {
      tNameB.setText("ID SALAH");
      return;
    }

    setBackLogin(id, op->name);
    return;
  }

}

void handleScanner() {
  while (SCANNER_SERIAL.available()) {
    char c = (char)SCANNER_SERIAL.read();

    if (c == '\n' || c == '\r') {
      if (scannerIdx > 0) {
        scannerBuffer[scannerIdx] = '\0';
        processScannerInput(scannerBuffer);
        scannerIdx = 0;
      }
      continue;
    }

    if (scannerIdx < sizeof(scannerBuffer) - 1) {
      scannerBuffer[scannerIdx] = c;
      scannerIdx++;
    }
  }
}
