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
NexButton bClearMF = NexButton(PAGE_MLD_F_ID, 4, "bClearMF");
NexButton bOkMF = NexButton(PAGE_MLD_F_ID, 5, "bOkMF");
NexText tInMF = NexText(PAGE_MLD_F_ID, 10, "tInMF");
NexPage pageMldB = NexPage(PAGE_MLD_B_ID, 0, "pageMldB");
NexButton bClearMB = NexButton(PAGE_MLD_B_ID, 4, "bClearMB");
NexButton bOkMB = NexButton(PAGE_MLD_B_ID, 5, "bOkMB");
NexText tInMB = NexText(PAGE_MLD_B_ID, 10, "tInMB");
NexPage pageLotF = NexPage(PAGE_LOT_F_ID, 0, "pageLotF");
NexButton bClearLotF = NexButton(PAGE_LOT_F_ID, 5, "bClearF");
NexButton bOkLotF = NexButton(PAGE_LOT_F_ID, 6, "bOkF");
NexText tInLotF = NexText(PAGE_LOT_F_ID, 11, "tInF");
NexNumber nIsiLotF = NexNumber(PAGE_LOT_F_ID, 12, "nIsiLotF");
NexPage pageLotB = NexPage(PAGE_LOT_B_ID, 0, "pageLotB");
NexButton bClearLotB = NexButton(PAGE_LOT_B_ID, 5, "bClearB");
NexButton bOkLotB = NexButton(PAGE_LOT_B_ID, 6, "bOkB");
NexText tInLotB = NexText(PAGE_LOT_B_ID, 11, "tInB");
NexNumber nIsiLotB = NexNumber(PAGE_LOT_B_ID, 12, "nIsiLotB");
NexPage pageOutputNG = NexPage(PAGE_OUTPUT_NG_ID, 0, "pageOutputNG");
NexPage pageInputNGF = NexPage(PAGE_INPUT_NG_F_ID, 0, "pageInputNGF");
NexPage pageInputNGB = NexPage(PAGE_INPUT_NG_B_ID, 0, "pageInputNGB");
NexPage pageDtF = NexPage(PAGE_DT_F_ID, 0, "pageDtF");
NexButton bBackDtF = NexButton(PAGE_DT_F_ID, 2, "bBackDtF");
NexButton bBhnF = NexButton(PAGE_DT_F_ID, 3, "bBhnF");
NexButton bTggPnsF = NexButton(PAGE_DT_F_ID, 4, "bTggPnsF");
NexButton bMoffF = NexButton(PAGE_DT_F_ID, 5, "bMoffF");
NexButton bLbhPnsF = NexButton(PAGE_DT_F_ID, 6, "bLbhPnsF");
NexButton bKrgPnsF = NexButton(PAGE_DT_F_ID, 7, "bKrgPnsF");
NexButton bRunF = NexButton(PAGE_DT_F_ID, 8, "bRunF");
NexButton bTrialF = NexButton(PAGE_DT_F_ID, 9, "bTrialF");

NexPage pageDtB = NexPage(PAGE_DT_B_ID, 0, "pageDtB");
NexButton bBackDtB = NexButton(PAGE_DT_B_ID, 2, "bBackDtB");
NexButton bBhnB = NexButton(PAGE_DT_B_ID, 3, "bBhnB");
NexButton bTggPnsB = NexButton(PAGE_DT_B_ID, 4, "bTggPnsB");
NexButton bMoffB = NexButton(PAGE_DT_B_ID, 5, "bMoffB");
NexButton bLbhPnsB = NexButton(PAGE_DT_B_ID, 6, "bLbhPnsB");
NexButton bKrgPnsB = NexButton(PAGE_DT_B_ID, 7, "bKrgPnsB");
NexButton bRunB = NexButton(PAGE_DT_B_ID, 8, "bRunB");
NexButton bTrialB = NexButton(PAGE_DT_B_ID, 9, "bTrialB");

NexPage pageDtInfoF = NexPage(PAGE_DT_INFO_F_ID, 0, "pageDtInfoF");
NexPage pageDtInfoB = NexPage(PAGE_DT_INFO_B_ID, 0, "pageDtInfoB");
NexPage pageDtMc = NexPage(PAGE_DT_MC_ID, 0, "pageDtMc");
NexButton bBackMc = NexButton(PAGE_DT_MC_ID, 2, "bBackMc");
NexButton bGMldMc = NexButton(PAGE_DT_MC_ID, 3, "bGMldMc");
NexButton bCMldMc = NexButton(PAGE_DT_MC_ID, 4, "bCMldMc");
NexButton bTrMc = NexButton(PAGE_DT_MC_ID, 5, "bTrMc");
NexButton bTrTlMc = NexButton(PAGE_DT_MC_ID, 6, "bTrTlMc");
NexButton bSetMc = NexButton(PAGE_DT_MC_ID, 7, "bSetMc");
NexButton bPrevMc = NexButton(PAGE_DT_MC_ID, 8, "bPrevMc");
NexButton bOffMc = NexButton(PAGE_DT_MC_ID, 9, "bOffMc");
NexButton bRunMc = NexButton(PAGE_DT_MC_ID, 10, "bRunMc");
NexButton bOrisolMc = NexButton(PAGE_DT_MC_ID, 11, "bOrisolMc");

NexPage pageDtMcInfo = NexPage(PAGE_DT_MC_INFO_ID, 0, "pageDtMcInfo");
NexPage pageSim = NexPage(PAGE_SIM_ID, 0, "pageSim");
NexButton bAutoEnF = NexButton(PAGE_SIM_ID, 35, "bAutoEnF");
NexButton bAutoDisF = NexButton(PAGE_SIM_ID, 36, "bAutoDisF");
NexButton bAutoEnB = NexButton(PAGE_SIM_ID, 37, "bAutoEnB");
NexButton bAutoDisB = NexButton(PAGE_SIM_ID, 38, "bAutoDisB");
NexPage pageSys = NexPage(PAGE_SYS_ID, 0, "pageSys");
NexVariable nFCavSys = NexVariable(PAGE_SYS_ID, 12, "nFCav");
NexVariable nBCavSys = NexVariable(PAGE_SYS_ID, 15, "nBCav");
NexPage keybdA = NexPage(PAGE_KEYBD_A_ID, 0, "keybdA");
NexPage keybdB = NexPage(PAGE_KEYBD_B_ID, 0, "keybdB");

NexTouch* nex_listen_list[] = {
  &pageDashboard,
  &pageLoginF, &bOkF, &bLogoutF, &bBackF, &tNameF, &nIdF,
  &pageLoginB, &bOkB, &bLogoutB, &bBackB, &tNameB, &nIdB,
  &pageMldF, &bClearMF, &bOkMF, &tInMF,
  &pageMldB, &bClearMB, &bOkMB, &tInMB,
  &pageLotF, &bClearLotF, &bOkLotF, &tInLotF, &nIsiLotF,
  &pageLotB, &bClearLotB, &bOkLotB, &tInLotB, &nIsiLotB,
  &pageOutputNG, &pageInputNGF, &pageInputNGB,
  &pageDtF, &bBackDtF, &bBhnF, &bTggPnsF, &bMoffF, &bLbhPnsF, &bKrgPnsF, &bRunF, &bTrialF,
  &pageDtB, &bBackDtB, &bBhnB, &bTggPnsB, &bMoffB, &bLbhPnsB, &bKrgPnsB, &bRunB, &bTrialB,
  &pageDtInfoF, &pageDtInfoB,
  &pageDtMc, &bBackMc, &bGMldMc, &bCMldMc, &bTrMc, &bTrTlMc, &bSetMc, &bPrevMc, &bOffMc, &bRunMc, &bOrisolMc,
  &pageDtMcInfo, &pageSim, &bAutoEnF, &bAutoDisF, &bAutoEnB, &bAutoDisB, &pageSys,
  &keybdA, &keybdB,
  NULL
};

void registerLoginCallbacks();
void registerInputCallbacks();
void registerDowntimeCallbacks();
void registerSimulationCallbacks();
void handleScanner();
void updateSimulation();
void initRtc();
void updateTnow();

void setup() {
  Serial.begin(115200);
  SCANNER_SERIAL.begin(SCANNER_BAUD, SERIAL_8N1, SCANNER_RX_PIN, SCANNER_TX_PIN);
  initRtc();
  registerLoginCallbacks();
  registerInputCallbacks();
  registerDowntimeCallbacks();
  registerSimulationCallbacks();
}

void loop() {
  nexLoop(nex_listen_list);
  handleScanner();
  updateTnow();
  updateSimulation();
}
