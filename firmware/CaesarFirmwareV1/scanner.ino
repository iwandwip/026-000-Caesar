#include <stdlib.h>

char scannerBuffer[32];
uint8_t scannerIdx = 0;

void processScannerInput(const char* input) {
  uint16_t id = (uint16_t)atoi(input);
  const Operator* op = findOperator(id);

  if (currentPageId == PAGE_LOGIN_F_ID) {
    if (op == nullptr) {
      tNameF.setText("ID SALAH");
      return;
    }

    setFrontLogin(id, op->name);
    return;
  }

  if (currentPageId == PAGE_LOGIN_B_ID) {
    if (op == nullptr) {
      tNameB.setText("ID SALAH");
      return;
    }

    setBackLogin(id, op->name);
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
