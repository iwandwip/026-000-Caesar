#include <stdlib.h>
char scannerBuffer[32];
uint8_t scannerIdx = 0;

void processScannerInput(const char* input) {
  if (currentPageId == PAGE_MLD_F_ID) {
    fillFrontMouldInput(input);
    return;
  }

  if (currentPageId == PAGE_MLD_B_ID) {
    fillBackMouldInput(input);
    return;
  }

  if (currentPageId == PAGE_LOT_F_ID) {
    fillFrontLotInput(input);
    return;
  }

  if (currentPageId == PAGE_LOT_B_ID) {
    fillBackLotInput(input);
    return;
  }

  if (currentPageId == PAGE_LOGIN_F_ID) {
    uint32_t id = strtoul(input, NULL, 10);
    nIdF.setValue(id);
    return;
  }

  if (currentPageId == PAGE_LOGIN_B_ID) {
    uint32_t id = strtoul(input, NULL, 10);
    nIdB.setValue(id);
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
