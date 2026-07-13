#include "config.h"
#include "Nextion.h"

const uint8_t PAGE_DASHBOARD_ID = 0;
const uint8_t PAGE_LOGIN_F_ID = 1;
const uint8_t PAGE_LOGIN_B_ID = 2;
const uint8_t PAGE_MLD_F_ID = 3;
const uint8_t PAGE_MLD_B_ID = 4;
const uint8_t PAGE_LOT_F_ID = 5;
const uint8_t PAGE_LOT_B_ID = 6;
const uint8_t PAGE_OUTPUT_NG_ID = 7;
const uint8_t PAGE_INPUT_NG_F_ID = 8;
const uint8_t PAGE_INPUT_NG_B_ID = 9;
const uint8_t PAGE_DT_F_ID = 10;
const uint8_t PAGE_DT_B_ID = 11;
const uint8_t PAGE_DT_INFO_F_ID = 12;
const uint8_t PAGE_DT_INFO_B_ID = 13;
const uint8_t PAGE_DT_MC_ID = 14;
const uint8_t PAGE_DT_MC_INFO_ID = 15;
const uint8_t PAGE_SIM_ID = 16;
const uint8_t PAGE_SYS_ID = 17;
const uint8_t PAGE_KEYBD_A_ID = 18;
const uint8_t PAGE_KEYBD_B_ID = 19;

uint8_t currentPageId = 0;

NexPage pageDashboard = NexPage(PAGE_DASHBOARD_ID, 0, "pageDashboard");
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

NexPage pageMldF = NexPage(PAGE_MLD_F_ID, 0, "pageMldF");
NexPage pageMldB = NexPage(PAGE_MLD_B_ID, 0, "pageMldB");
NexPage pageLotF = NexPage(PAGE_LOT_F_ID, 0, "pageLotF");
NexPage pageLotB = NexPage(PAGE_LOT_B_ID, 0, "pageLotB");
NexPage pageOutputNG = NexPage(PAGE_OUTPUT_NG_ID, 0, "pageOutputNG");
NexPage pageInputNGF = NexPage(PAGE_INPUT_NG_F_ID, 0, "pageInputNGF");
NexPage pageInputNGB = NexPage(PAGE_INPUT_NG_B_ID, 0, "pageInputNGB");
NexPage pageDtF = NexPage(PAGE_DT_F_ID, 0, "pageDtF");
NexPage pageDtB = NexPage(PAGE_DT_B_ID, 0, "pageDtB");
NexPage pageDtInfoF = NexPage(PAGE_DT_INFO_F_ID, 0, "pageDtInfoF");
NexPage pageDtInfoB = NexPage(PAGE_DT_INFO_B_ID, 0, "pageDtInfoB");
NexPage pageDtMc = NexPage(PAGE_DT_MC_ID, 0, "pageDtMc");
NexPage pageDtMcInfo = NexPage(PAGE_DT_MC_INFO_ID, 0, "pageDtMcInfo");
NexPage pageSim = NexPage(PAGE_SIM_ID, 0, "pageSim");
NexPage pageSys = NexPage(PAGE_SYS_ID, 0, "pageSys");
NexPage keybdA = NexPage(PAGE_KEYBD_A_ID, 0, "keybdA");
NexPage keybdB = NexPage(PAGE_KEYBD_B_ID, 0, "keybdB");

NexTouch* nex_listen_list[] = {
  &pageDashboard,
  &pageLoginF, &bOkF, &bLogoutF, &bBackF, &tNameF, &nIdF,
  &pageLoginB, &bOkB, &bLogoutB, &bBackB, &tNameB, &nIdB,
  &pageMldF, &pageMldB, &pageLotF, &pageLotB,
  &pageOutputNG, &pageInputNGF, &pageInputNGB,
  &pageDtF, &pageDtB, &pageDtInfoF, &pageDtInfoB,
  &pageDtMc, &pageDtMcInfo, &pageSim, &pageSys,
  &keybdA, &keybdB,
  NULL
};

void registerLoginCallbacks();
void handleScanner();

void setup() {
  Serial.begin(115200);
  SCANNER_SERIAL.begin(SCANNER_BAUD, SERIAL_8N1, SCANNER_RX_PIN, SCANNER_TX_PIN);
  registerLoginCallbacks();
}

void loop() {
  nexLoop(nex_listen_list);
  handleScanner();
}
