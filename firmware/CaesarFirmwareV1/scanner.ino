#include <stdlib.h>

char scannerBuffer[32];
uint8_t scannerIdx = 0;

void processScannerInput(const char* input) {
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

  nIdF.setValue(id);
  nIdB.setValue(id);
  tNameF.setText("PAGE?");
  tNameB.setText("PAGE?");
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
