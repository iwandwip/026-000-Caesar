//Update program 11-03-2026 FUNGSI BACK (CHECK LOT)

#include "Nextion.h"
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include "uRTCLib.h"
#include "time.h"
#include <ArduinoJson.h>
#include <ESP32HTTPUpdateServer.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPUpdateServer.h>

//WiFi and MQTT Configuration
const char* ssid             = "P2-AP"; //Ruckus_Finishing_2X //Ruckus_Barcode_Plant2X //Ruckus_Finishing_1
const char* password         = "indoprima";
const char* mqtt_server      = "192.168.10.10";
const int mqtt_port          = 1883;
const char* topic            = "SF-HBP/";
const char* mqtt_topic       = "BP/";

//Address Configuration
const int AddressPin[6]      = {5,35,34,39,36,23};       //BOARD BARU
const int ioPin[9]           = {19,18,32,33,25,26,27,14,12};  //WARNA HIJAU
#define   input1      digitalRead(19)
#define   input2      digitalRead(18)
#define   input3      digitalRead(32)
#define   input4      digitalRead(33)
#define   input5      digitalRead(25)
#define   input6      digitalRead(26)
#define   input7      digitalRead(27)
#define   input8      digitalRead(14)
#define   input9      digitalRead(12)

int address                  = 0;

//OTA Credential
const char* ota_username     = "admin";
const char* ota_password     = "123654789";

WiFiClient espClient;
PubSubClient client(espClient);
LiquidCrystal_I2C lcd(0x27, 16, 4);
uRTCLib rtc(0x68);
char lcdBuff[20];
WebServer httpServer(80);
ESP32HTTPUpdateServer httpUpdater;

String esp_client;
String full_topic;
String full_mqtt_topic;
String control_mqtt_topic;
String read_mqtt_topic;
String wifi_mqtt_topic;
String ip_mqtt_topic;
String idnama_mqtt_topic;
String dataA_mqtt_topic;
String dataB_mqtt_topic;
String revdataA_mqtt_topic;
String revdataB_mqtt_topic;
String cycle_mqtt_topic;

char noregProd[8], namaProd[45];
char input_jenis[20];
uint32_t input_jenisPcs;

unsigned long lastChangeTime            = 0;
unsigned long lastBlinkTime             = 0;
bool ledState                           = LOW;
unsigned long previousMillis            = 0;
const long interval                     = 3000;
unsigned long lastReconnectAttempt      = 0;
volatile unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay       = 50;
volatile int resetCount                 = 0;
volatile bool shouldPublishCounter      = false;
bool isScannerReady                     = true;
const int buzzerPin                     = 13;
const int counterPin                    = 19;

String inputs_loginNoreg;
String inputs_lotWo;
String inputs_mldKode;

char aNoreg[20], bNoreg[20];
int dtMcNo         = 0;
int currentLayer   = 0; // 0 = A, 1 = B

//=============LAYER A============//
char aIdfgs[25], aModel[60], aLotWo[11], aLotWg[13], aLotWgDbl[13];
uint32_t aIdfgs_length, aModel_length, aLotWo_length, aLotWg_length, aLotWgDbl_length;
uint32_t aLotPcs, aLotKuota, aLotIsi, aLotOk, aLotNg, aLotTotal, aFlagDbl;
char aIdfgs2[25], aModel2[60], aLotWo2[11], aLotWg2[13], aLotWgDbl2[13];
uint32_t aIdfgs2_length, aModel2_length, aLotWo2_length, aLotWg2_length, aLotWgDbl2_length;
uint32_t aLotPcs2, aLotKuota2, aLotIsi2, aLotOk2, aLotNg2, aLotTotal2, aFlagDbl2;
char aMldKode[7], aMldModel[35], aDtLayerStart[21], aDtLayerFinish[21], aDtLayerJenis[15];
uint32_t aMldKode_length, aMldModel_length, aMldCav, aMldIsi, aDtLayerStart_length, aDtLayerFinish_length, aDtLayerNo, aDtLayerJenis_length;

//=============LAYER B============//
char bIdfgs[25], bModel[60], bLotWo[11], bLotWg[13], bLotWgDbl[13];
uint32_t bIdfgs_length, bModel_length, bLotWo_length, bLotWg_length, bLotWgDbl_length;
uint32_t bLotPcs, bLotKuota, bLotIsi, bLotOk, bLotNg, bLotTotal, bFlagDbl;
char bIdfgs2[25], bModel2[60], bLotWo2[11], bLotWg2[13], bLotWgDbl2[13];
uint32_t bIdfgs2_length, bModel2_length, bLotWo2_length, bLotWg2_length, bLotWgDbl2_length;
uint32_t bLotPcs2, bLotKuota2, bLotIsi2, bLotOk2, bLotNg2, bLotTotal2, bFlagDbl2;
char bMldKode[7], bMldModel[35], bDtLayerStart[21], bDtLayerFinish[21], bDtLayerJenis[15];
uint32_t bMldKode_length, bMldModel_length, bMldCav, bMldIsi, bDtLayerStart_length, bDtLayerFinish_length, bDtLayerNo, bDtLayerJenis_length;

char dtMcStart[21], dtMcFinish[21], dtMcJenis[15];
uint32_t dtMcStart_length, dtMcFinish_length, dtMcJenis_length;
char warnB1[40], warnB2[40], warnB3[40], warnB4[40], warnB5[40], warnB6[40];

char input_lotWo[15], input_lotWg[13], input_lotWgDbl[13];
char input_mldKode[7], input_ng[20], input_dtLayer[15], input_dtMc[20];
char input_loginNoreg[8];
char lastNoreg[8] = "";

char kosong[10];
int count = 0, tim_count = 0; // Inisialisasi count dan tim_count
char bufferA[100], bufferB[100], buff[10], buff_count[50];
int flagModel, flagLot, flagLayer, flagMld, flagIsi, flagIsiKirim, flagNoreg, flagLogin, flagCav, flagOutput, flagSwitch, flagDtLayer, flagDtMc, flagDtMcStart, flagCycle;
int flag_aDtLayerStart, flag_bDtLayerStart, flag_cDtLayerStart, flag_dDtLayerStart, flag_eDtLayerStart;
int flag_aDtLayer, flag_bDtLayer, flag_cDtLayer, flag_dDtLayer, flag_eDtLayer;
int flag_aLot2, flag_bLot2, flag_cLot2, flag_dLot2, flag_eLot2;
int flagGantiMld, flagGantiKirim;
bool flagRun, flagStop, flagCekRun, flagStart;
uint32_t aCekKuota, bCekKuota, cCekKuota, dCekKuota, eCekKuota;
int ready;
char startTime[21];
int jmlLayer;

bool scanNoregMode = false;
bool scanLotMode   = false;

//====================PAGE MAIN====================//
NexPage pageMain       = NexPage(0, 0, "pageMain");
  
NexButton b_aLayer     = NexButton(0, 10, "aLayer");       NexText t_aLayer    = NexText(0, 10, "aLayer");
NexButton b_aModel     = NexButton(0, 11, "aModel");       NexText t_aModel    = NexText(0, 11, "aModel");
NexButton b_aCav       = NexButton(0, 40, "aCav");         NexNumber n_aCav    = NexNumber(0, 40, "aCav");
NexButton b_aIsi       = NexButton(0, 41, "aIsi");         NexNumber n_aIsi    = NexNumber(0, 41, "aIsi");
NexButton b_aOutput    = NexButton(0, 42, "aOutput");      NexNumber n_aOutput = NexNumber(0, 42, "aOutput");
NexButton b_aNg        = NexButton(0, 55, "aNg");          NexNumber n_aNg     = NexNumber(0, 55, "aNg");
    
NexButton b_bLayer     = NexButton(0, 12, "bLayer");       NexText t_bLayer    = NexText(0, 12, "bLayer");
NexButton b_bModel     = NexButton(0, 13, "bModel");       NexText t_bModel    = NexText(0, 13, "bModel");
NexButton b_bCav       = NexButton(0, 43, "bCav");         NexNumber n_bCav    = NexNumber(0, 43, "bCav");
NexButton b_bIsi       = NexButton(0, 44, "bIsi");         NexNumber n_bIsi    = NexNumber(0, 44, "bIsi");
NexButton b_bOutput    = NexButton(0, 45, "bOutput");      NexNumber n_bOutput = NexNumber(0, 45, "bOutput");
NexButton b_bNg        = NexButton(0, 57, "bNg");          NexNumber n_bNg     = NexNumber(0, 57, "bNg");
  
NexNumber n_target     = NexNumber(0, 9, "target");
NexNumber n_aktual     = NexNumber(0, 7, "aktual");
NexNumber n_output     = NexNumber(0, 5, "output");
NexButton b_iconSwitch = NexButton(0, 39, "iconSwitch");

NexButton b_indikator  = NexButton(0, 46, "indikator");    NexText t_indikator  = NexText(0, 46, "indikator");
NexButton b_operator1  = NexButton(0, 16, "operator1");    NexText t_operator1  = NexText(0, 16, "operator1");
NexButton b_iconLogin1 = NexButton(0, 17, "iconLogin1");   NexText t_iconLogin1 = NexText(0, 17, "iconLogin1");
NexButton b_operator2  = NexButton(0, 48, "operator2");    NexText t_operator2  = NexText(0, 48, "operator2");
NexButton b_iconLogin2 = NexButton(0, 49, "iconLogin2");   NexText t_iconLogin2 = NexText(0, 49, "iconLogin2");

NexTimer tm1           = NexTimer(0, 28, "tm1");
NexVariable v_va0      = NexVariable(0, 19, "va0");
NexVariable v_va1      = NexVariable(0, 29, "va1");
NexVariable v_va2      = NexVariable(0, 30, "va2");
NexVariable v_va3      = NexVariable(0, 31, "va3");
NexVariable v_va4      = NexVariable(0, 32, "va4");
NexVariable v_va5      = NexVariable(0, 33, "va5");
NexVariable v_va6      = NexVariable(0, 34, "va6");

int flag_pageMain;
void pageMainPop(void *ptr); 
void b_aLayerRls(void *ptr); void b_aModelRls(void *ptr); void b_aCavRls(void *ptr); void b_aIsiRls(void *ptr); void b_aOutputRls(void *ptr); void b_aNgRls(void *ptr); 
void b_bLayerRls(void *ptr); void b_bModelRls(void *ptr); void b_bCavRls(void *ptr); void b_bIsiRls(void *ptr); void b_bOutputRls(void *ptr); void b_bNgRls(void *ptr); 
void b_iconSwitchRls(void *ptr); void b_operator1Rls(void *ptr); void b_iconLogin1Rls(void *ptr); 
void b_indikatorRls(void *ptr); void b_operator2Rls(void *ptr); void b_iconLogin2Rls(void *ptr);
//=========================================================================================================================================//

//====================PAGE KEY A====================//
NexPage keybdA    = NexPage(1, 0, "keybdA");

NexButton b_b0a   = NexButton(1, 3, "b0");    NexButton b_b1a   = NexButton(1, 6, "b1");    NexButton b_b2a   = NexButton(1, 7, "b2");    NexButton b_b3a   = NexButton(1, 8, "b3");
NexButton b_b4a   = NexButton(1, 9, "b4");    NexButton b_b5a   = NexButton(1, 10, "b5");   NexButton b_b6a   = NexButton(1, 11, "b6");   NexButton b_b7a   = NexButton(1, 12, "b7");
NexButton b_b8a   = NexButton(1, 13, "b8");   NexButton b_b9a   = NexButton(1, 47, "b9");   NexButton b_b20a  = NexButton(1, 15, "b20");  NexButton b_b21a  = NexButton(1, 16, "b21");
NexButton b_b22a  = NexButton(1, 17, "b22");  NexButton b_b23a  = NexButton(1, 18, "b23");  NexButton b_b24a  = NexButton(1, 19, "b24");  NexButton b_b25a  = NexButton(1, 20, "b25");
NexButton b_b26a  = NexButton(1, 21, "b26");  NexButton b_b27a  = NexButton(1, 22, "b27");  NexButton b_b28a  = NexButton(1, 28, "b28");  NexButton b_b40a  = NexButton(1, 25, "b40");
NexButton b_b41a  = NexButton(1, 26, "b41");  NexButton b_b42a  = NexButton(1, 27, "b42");  NexButton b_b43a  = NexButton(1, 28, "b43");  NexButton b_b44a  = NexButton(1, 29, "b44");
NexButton b_b45a  = NexButton(1, 30, "b45");  NexButton b_b46a  = NexButton(1, 31, "b46");  NexButton b_b200a = NexButton(1, 14, "b200"); NexButton b_b210a = NexButton(1, 5, "b210");
NexButton b_b220a = NexButton(1, 24, "b220"); NexButton b_b240a = NexButton(1, 33, "b240"); NexButton b_b251a = NexButton(1, 4, "b251");

void keybdAPop(void *ptr);
void b_b0aRls(void *ptr);   void b_b1aRls(void *ptr);   void b_b2aRls(void *ptr);   void b_b3aRls(void *ptr);   void b_b4aRls(void *ptr);   void b_b5aRls(void *ptr);   void b_b6aRls(void *ptr);
void b_b7aRls(void *ptr);   void b_b8aRls(void *ptr);   void b_b9aRls(void *ptr);   void b_b20aRls(void *ptr);  void b_b21aRls(void *ptr);  void b_b22aRls(void *ptr);  void b_b23aRls(void *ptr);
void b_b24aRls(void *ptr);  void b_b25aRls(void *ptr);  void b_b26aRls(void *ptr);  void b_b27aRls(void *ptr);  void b_b28aRls(void *ptr);  void b_b40aRls(void *ptr);  void b_b41aRls(void *ptr);
void b_b42aRls(void *ptr);  void b_b43aRls(void *ptr);  void b_b44aRls(void *ptr);  void b_b45aRls(void *ptr);  void b_b46aRls(void *ptr);  void b_b200aRls(void *ptr); void b_b210aRls(void *ptr);
void b_b220aRls(void *ptr); void b_b240aRls(void *ptr); void b_b251aRls(void *ptr);
//=========================================================================================================================================//

//====================PAGE KEY B====================//
NexPage keybdB   = NexPage(2, 0, "keybdB");
  
NexButton b_b0   = NexButton(2, 2, "b0");   NexButton b_b1 = NexButton(2, 5, "b1");   NexButton b_b2   = NexButton(2, 6, "b2");     NexButton b_b3   = NexButton(2, 7, "b3");
NexButton b_b4   = NexButton(2, 8, "b4");   NexButton b_b5 = NexButton(2, 9, "b5");   NexButton b_b6   = NexButton(2, 10, "b6");    NexButton b_b7   = NexButton(2, 11, "b7");
NexButton b_b8   = NexButton(2, 12, "b8");  NexButton b_b9 = NexButton(2, 14, "b9");  NexButton b_b200 = NexButton(2, 13, "b200");  NexButton b_b210 = NexButton(2, 4, "b210");
NexButton b_b251 = NexButton(2, 22, "b251");

void keybdBPop(void *ptr);
void b_b0Rls(void *ptr); void b_b1Rls(void *ptr); void b_b2Rls(void *ptr); void b_b3Rls(void *ptr); void b_b4Rls(void *ptr); void b_b5Rls(void *ptr);
void b_b6Rls(void *ptr); void b_b7Rls(void *ptr); void b_b8Rls(void *ptr); void b_b9Rls(void *ptr); void b_b200Rls(void *ptr); void b_b210Rls(void *ptr); void b_b251Rls(void *ptr);
//=========================================================================================================================================//

//====================PAGE LOGIN====================//
NexPage pageLogin       = NexPage(3, 0, "pageLogin");

NexButton b_loginNoreg  = NexButton(3, 7, "loginNoreg");      NexText t_loginNoreg  = NexText(3, 7, "loginNoreg");
NexButton b_loginNama   = NexButton(3, 8, "loginNama");       NexText t_loginNama   = NexText(3, 8, "loginNama");
NexButton b_loginLogout = NexButton(3, 3, "loginLogout");     NexText t_loginLogout = NexText(3, 3, "loginLogout");
NexButton b_loginOk     = NexButton(3, 2, "loginOk");         NexText t_loginOk     = NexText(3, 2, "loginOk");
NexButton b_loginClose  = NexButton(3, 4, "loginClose");      NexText t_loginClose  = NexText(3, 4, "loginClose");

void pageLoginPop(void *ptr);
void b_loginNoregRls(void *ptr); void b_loginNamaRls(void *ptr); void b_loginLogoutRls(void *ptr);
void b_loginOkRls(void *ptr); void b_loginCloseRls(void *ptr);
//=========================================================================================================================================//

//====================PAGE LOT====================//
NexPage pageLot        = NexPage(4, 0, "pageLot");

NexButton b_lotWo      = NexButton(4, 7, "lotWo");           NexText t_lotWo      = NexText(4, 7, "lotWo");
NexButton b_lotWg      = NexButton(4, 10, "lotWg");          NexText t_lotWg      = NexText(4, 10, "lotWg");
NexButton b_lotPcs     = NexButton(4, 22, "lotPcs");         NexNumber n_lotPcs   = NexNumber(4, 22, "lotPcs");
NexButton b_lotKuota   = NexButton(4, 29, "lotKuota");       NexNumber n_lotKuota = NexNumber(4, 29, "lotKuota");
NexButton b_lotIsi     = NexButton(4, 23, "lotIsi");         NexNumber n_lotIsi   = NexNumber(4, 23, "lotIsi");
NexButton b_lotCav     = NexButton(4, 26, "lotCav");         NexNumber n_lotCav   = NexNumber(4, 26, "lotCav");

NexButton b_lotSubmit  = NexButton(4, 8, "lotSubmit");       NexText t_lotSubmit  = NexText(4, 8, "lotSubmit");
NexButton b_lotOk      = NexButton(4, 9, "lotOk");           NexText t_lotOk      = NexText(4, 9, "lotOk");
NexButton b_lotClose   = NexButton(4, 12, "lotClose");       NexText t_lotClose   = NexText(4, 12, "lotClose");

NexDSButton dsb_lotCpA = NexDSButton(4, 31, "lotCpA");
NexDSButton dsb_lotCpB = NexDSButton(4, 32, "lotCpB");
NexDSButton dsb_lotCpC = NexDSButton(4, 33, "lotCpC");
NexDSButton dsb_lotCpD = NexDSButton(4, 34, "lotCpD");
NexDSButton dsb_lotCpE = NexDSButton(4, 35, "lotCpE");

void pageLotPop(void *ptr);
void b_lotWoRls(void *ptr); void b_lotWgRls(void *ptr); void b_lotPcsRls(void *ptr); void b_lotKuotaRls(void *ptr); void b_lotIsiRls(void *ptr);
void b_lotCavRls(void *ptr); void b_lotSubmitRls(void *ptr); void b_lotOkRls(void *ptr);  void b_lotCloseRls(void *ptr);
// void dsb_lotCpARls(void *ptr); void dsb_lotCpBRls(void *ptr); void dsb_lotCpCRls(void *ptr); void dsb_lotCpDRls(void *ptr); void dsb_lotCpERls(void *ptr);

int flag_pageLot;
//=========================================================================================================================================//

//====================PAGE MOULDING====================//
NexPage pageMld = NexPage(5, 0, "pageMld");

NexButton b_mldKode  = NexButton(5, 3, "mldKode");      NexText t_mldKode  = NexText(5, 3, "mldKode");
NexButton b_mldModel = NexButton(5, 7, "mldModel");     NexText t_mldModel = NexText(5, 7, "mldModel");
NexButton b_mldCav   = NexButton(5, 11, "mldCav");      NexNumber n_mldCav = NexNumber(5, 11, "mldCav");
NexButton b_mldIsi   = NexButton(5, 12, "mldIsi");      NexNumber n_mldIsi = NexNumber(5, 12, "mldIsi");
NexButton b_mldClear = NexButton(5, 4, "mldClear");     NexText t_mldClear = NexText(5, 4, "mldClear");
NexButton b_mldOk    = NexButton(5, 5, "mldOk");        NexText t_mldOk    = NexText(5, 5, "mldOk");
NexButton b_mldClose = NexButton(5, 9, "mldClose");     NexText t_mldClose = NexText(5, 9, "mldClose");

void pageMldPop(void *ptr);
void b_mldKodeRls(void *ptr); void b_mldModelRls(void *ptr); void b_mldCavRls(void *ptr); void b_mldIsiRls(void *ptr);
void b_mldClearRls(void *ptr); void b_mldOkRls(void *ptr);  void b_mldCloseRls(void *ptr);
//=========================================================================================================================================//

//====================PAGE OUTPUT====================//
NexPage pageOutput    = NexPage(6, 0, "pageOutput");

NexButton b_outOk     = NexButton(6, 10, "outOk");            NexNumber n_outOk    = NexNumber(6, 10, "outOk");
NexButton b_outNg     = NexButton(6, 11, "outNg");            NexNumber n_outNg    = NexNumber(6, 11, "outNg");
NexButton b_outTotal  = NexButton(6, 12, "outTotal");         NexNumber n_outTotal = NexNumber(6, 12, "outTotal");
NexButton b_outJenis  = NexButton(6, 4, "outJenis");          NexText t_outJenis   = NexText(6, 4, "outJenis");
NexButton b_outPcs    = NexButton(6, 13, "outPcs");           NexNumber n_outPcs   = NexNumber(6, 13, "outPcs");
NexButton b_outSubmit = NexButton(6, 5, "outSubmit");         NexText t_outSubmit  = NexText(6, 5, "outSubmit");
NexButton b_outClose  = NexButton(6, 6, "outClose");          NexText t_outClose   = NexText(6, 6, "outClose");

int flag_pageOutput;
void pageOutputPop(void *ptr);
void b_outOkRls(void *ptr); void b_outNgRls(void *ptr); void b_outTotalRls(void *ptr); void b_outJenisRls(void *ptr); void b_outPcsRls(void *ptr); void b_outSubmitRls(void *ptr);
void b_outOk2Rls(void *ptr); void b_outNg2Rls(void *ptr); void b_outTotal2Rls(void *ptr); void b_outJenis2Rls(void *ptr); void b_outPcs2Rls(void *ptr); void b_outSubmit2Rls(void *ptr);
void b_outCloseRls(void *ptr);
//=========================================================================================================================================//

//====================PAGE NG====================//
NexPage pageNg = NexPage(7, 0, "pageNg");

NexButton b_ng1     = NexButton(7, 3, "ng1");          NexText t_ng1     = NexText(7, 3, "ng1");
NexButton b_ng2     = NexButton(7, 4, "ng2");          NexText t_ng2     = NexText(7, 4, "ng2");
NexButton b_ng3     = NexButton(7, 5, "ng3");          NexText t_ng3     = NexText(7, 5, "ng3");
NexButton b_ng4     = NexButton(7, 6, "ng4");          NexText t_ng4     = NexText(7, 6, "ng4");
NexButton b_ng5     = NexButton(7, 7, "ng5");          NexText t_ng5     = NexText(7, 7, "ng5");
NexButton b_ng6     = NexButton(7, 8, "ng6");          NexText t_ng6     = NexText(7, 8, "ng6");
NexButton b_ng7     = NexButton(7, 9, "ng7");          NexText t_ng7     = NexText(7, 9, "ng7");
NexButton b_ng8     = NexButton(7, 10, "ng8");         NexText t_ng8     = NexText(7, 10, "ng8");
NexButton b_ng9     = NexButton(7, 11, "ng9");         NexText t_ng9     = NexText(7, 11, "ng9");
NexButton b_ng10    = NexButton(7, 12, "ng10");        NexText t_ng10    = NexText(7, 12, "ng10");
NexButton b_ng11    = NexButton(7, 13, "ng11");        NexText t_ng11    = NexText(7, 13, "ng11");
NexButton b_ng12    = NexButton(7, 14, "ng12");        NexText t_ng12    = NexText(7, 14, "ng12");
NexButton b_ngClose = NexButton(7, 2, "ngClose");      NexText t_ngClose = NexText(7, 2, "ngClose");

void pageNgPop(void *ptr);
void b_ng1Rls(void *ptr); void b_ng2Rls(void *ptr); void b_ng3Rls(void *ptr); void b_ng4Rls(void *ptr);
void b_ng5Rls(void *ptr); void b_ng6Rls(void *ptr); void b_ng7Rls(void *ptr); void b_ng8Rls(void *ptr);
void b_ng9Rls(void *ptr); void b_ng10Rls(void *ptr); void b_ng11Rls(void *ptr); void b_ng12Rls(void *ptr);
void b_ngCloseRls(void *ptr);
//=========================================================================================================================================//

//====================PAGE DT LAYER====================//
NexPage pageDtLayer      = NexPage(8, 0, "pageDtLayer");

NexButton b_dtLayer1     = NexButton(8, 3, "dtLayer1");          NexText t_dtLayer1     = NexText(8, 3, "dtLayer1");
NexButton b_dtLayer2     = NexButton(8, 4, "dtLayer2");          NexText t_dtLayer2     = NexText(8, 4, "dtLayer2");
NexButton b_dtLayer3     = NexButton(8, 5, "dtLayer3");          NexText t_dtLayer3     = NexText(8, 5, "dtLayer3");
NexButton b_dtLayer4     = NexButton(8, 6, "dtLayer4");          NexText t_dtLayer4     = NexText(8, 6, "dtLayer4");
NexButton b_dtLayer5     = NexButton(8, 7, "dtLayer5");          NexText t_dtLayer5     = NexText(8, 7, "dtLayer5");
NexButton b_dtLayer6     = NexButton(8, 9, "dtLayer6");          NexText t_dtLayer6     = NexText(8, 9, "dtLayer6");
NexButton b_dtLayerRun   = NexButton(8, 8, "dtLayerRun");        NexText t_dtLayerRun   = NexText(8, 8, "dtLayerRun");
NexButton b_dtLayerClose = NexButton(8, 2, "dtLayerClose");      NexText t_dtLayerClose = NexText(8, 2, "dtLayerClose");

void pageDtLayerPop(void *ptr);
void b_dtLayer1Rls(void *ptr); void b_dtLayer2Rls(void *ptr); void b_dtLayer3Rls(void *ptr); void b_dtLayer4Rls(void *ptr);
void b_dtLayer5Rls(void *ptr); void b_dtLayer6Rls(void *ptr); void b_dtLayerRunRls(void *ptr); void b_dtLayerCloseRls(void *ptr);
//=========================================================================================================================================//

//====================PAGE DT MC====================//
NexPage pageDtMc      = NexPage(9, 0, "pageDtMc");

NexButton b_dtMc1     = NexButton(9, 3, "dtMc1");          NexText t_dtMc1     = NexText(9, 3, "dtMc1");
NexButton b_dtMc2     = NexButton(9, 4, "dtMc2");          NexText t_dtMc2     = NexText(9, 4, "dtMc2");
NexButton b_dtMc3     = NexButton(9, 5, "dtMc3");          NexText t_dtMc3     = NexText(9, 5, "dtMc3");
NexButton b_dtMc4     = NexButton(9, 6, "dtMc4");          NexText t_dtMc4     = NexText(9, 6, "dtMc4");
NexButton b_dtMc5     = NexButton(9, 7, "dtMc5");          NexText t_dtMc5     = NexText(9, 7, "dtMc5");
NexButton b_dtMc6     = NexButton(9, 8, "dtMc6");          NexText t_dtMc6     = NexText(9, 8, "dtMc6");
NexButton b_dtMc7     = NexButton(9, 9, "dtMc7");          NexText t_dtMc7     = NexText(9, 9, "dtMc7");
NexButton b_dtMc8     = NexButton(9, 11, "dtMc8");         NexText t_dtMc8     = NexText(9, 11, "dtMc8");
NexButton b_dtMc9     = NexButton(9, 12, "dtMc9");         NexText t_dtMc9     = NexText(9, 12, "dtMc9");
NexButton b_dtMcRun   = NexButton(9, 10, "dtMcRun");       NexText t_dtMcRun   = NexText(9, 10, "dtMcRun");
NexButton b_dtMcClose = NexButton(9, 2, "dtMcClose");      NexText t_dtMcClose = NexText(9, 2, "dtMcClose");

void pageDtMcPop(void *ptr);
void b_dtMc1Rls(void *ptr); void b_dtMc2Rls(void *ptr); void b_dtMc3Rls(void *ptr); void b_dtMc4Rls(void *ptr); void b_dtMc5Rls(void *ptr);
void b_dtMc6Rls(void *ptr); void b_dtMc7Rls(void *ptr); void b_dtMc8Rls(void *ptr); void b_dtMc9Rls(void *ptr);
void b_dtMcRunRls(void *ptr); void b_dtMcCloseRls(void *ptr);
//=========================================================================================================================================//

//====================PAGE DT INFO====================//
NexPage pageDtInfo       = NexPage(10, 0, "pageDtInfo");
NexButton b_dtInfo       = NexButton(10, 1, "dtInfo");         NexText t_dtInfo       = NexText(10, 1, "dtInfo");
NexButton b_dtInfoStart  = NexButton(10, 2, "dtInfoStart");    NexText t_dtInfoStart  = NexText(10, 2, "dtInfoStart");
NexButton b_dtInfoSwitch = NexButton(10, 4, "dtInfoSwitch");   NexText t_dtInfoSwitch = NexText(10, 4, "dtInfoSwitch");

int flag_pageDtInfo;
void pageDtInfoPop(void *ptr);
void b_dtInfoRls(void *ptr); void b_dtInfoStartRls(void *ptr); void b_dtInfoSwitchRls(void *ptr);
//=========================================================================================================================================//

//====================PAGE WARNING====================//
NexPage pageWarn       = NexPage(11, 0, "pageWarn");
NexButton b_warnBorder = NexButton(11, 1, "warnBorder");        NexText t_warnBorder = NexText(11, 1, "warnBorder");
NexButton b_warnB1     = NexButton(11, 2, "warnB1");            NexText t_warnB1     = NexText(11, 2, "warnB1");
NexButton b_warnB2     = NexButton(11, 3, "warnB2");            NexText t_warnB2     = NexText(11, 3, "warnB2");
NexButton b_warnB3     = NexButton(11, 4, "warnB3");            NexText t_warnB3     = NexText(11, 4, "warnB3");
NexButton b_warnB4     = NexButton(11, 5, "warnB4");            NexText t_warnB4     = NexText(11, 5, "warnB4");
NexButton b_warnB5     = NexButton(11, 6, "warnB5");            NexText t_warnB5     = NexText(11, 6, "warnB5");
NexButton b_warnB6     = NexButton(11, 7, "warnB6");            NexText t_warnB6     = NexText(11, 7, "warnB6");
NexButton b_warnOk     = NexButton(11, 8, "warnOk");            NexText t_warnOk     = NexText(11, 8, "warnOk");

int flag_pageWarn=0;
uint32_t colorB1, colorB2, colorB3, colorB4, colorB5, colorB6;
void pageWarnPop(void *ptr);
void b_warnOkRls(void *ptr);
//=========================================================================================================================================//


NexTouch *nex_listen_list[] = {
  //====================PAGE MAIN====================//
  &pageMain,
  &b_aLayer, &b_aModel, &b_aCav, &b_aIsi, &b_aOutput, &b_bLayer, &b_bModel, &b_bCav, &b_bIsi, &b_bOutput, &b_aNg, &b_bNg ,
  &t_aLayer, &t_aModel, &n_aCav, &n_aIsi, &n_aOutput, &t_bLayer, &t_bModel, &n_bCav, &n_bIsi, &n_bOutput, &n_aNg, &n_bNg,
  &n_target, &n_aktual, &n_output, &b_iconSwitch, &b_operator1, &t_operator1, &b_operator2, &t_operator2, &b_iconLogin1, &t_iconLogin1, 
  &b_iconLogin2, &t_iconLogin2, &b_indikator, &tm1,
  //====================PAGE KEY A====================//
  &keybdA,
  &b_b0a, &b_b1a, &b_b2a, &b_b3a, &b_b4a, &b_b5a, &b_b6a, &b_b7a, &b_b8a, &b_b9a, &b_b20a, &b_b21a, &b_b22a, &b_b23a,
  &b_b24a, &b_b25a, &b_b26a, &b_b27a, &b_b28a, &b_b40a, &b_b41a, &b_b42a, &b_b43a, &b_b44a, &b_b45a, &b_b46a, &b_b200a,
  &b_b210a, &b_b220a, &b_b240a, &b_b251a,
  
  //====================PAGE KEY B====================//
  &keybdB,
  &b_b0, &b_b1, &b_b2, &b_b3, &b_b4, &b_b5, &b_b6, &b_b7, &b_b8, &b_b9, &b_b200, &b_b210, &b_b251,
  //====================PAGE LOGIN====================//
  &pageLogin,
  &b_loginNoreg, &b_loginNama, &b_loginLogout, &b_loginOk, &b_loginClose, 
  &t_loginNoreg, &t_loginNama, &t_loginLogout, &t_loginOk, &t_loginClose,
  //====================PAGE LOT====================//
  &pageLot,
  &b_lotWo, &b_lotWg, &b_lotPcs, &b_lotKuota, &b_lotIsi,
  &t_lotWo, &t_lotWg, &n_lotPcs, &n_lotKuota, &n_lotIsi,
  &n_lotCav, &t_lotSubmit, &t_lotOk, &t_lotClose,
  &dsb_lotCpA, &dsb_lotCpB, &dsb_lotCpC, &dsb_lotCpD, &dsb_lotCpE,
  //====================PAGE MOULDING====================//
  &pageMld,
  &b_mldKode, &b_mldModel, &b_mldCav, &b_mldIsi, &b_mldClear, &b_mldOk, &b_mldClose,
  //====================PAGE OUTPUT====================//
  &pageOutput,
  &b_outOk, &b_outNg, &b_outTotal, &b_outJenis, &b_outPcs, &b_outSubmit,
  &n_outOk, &n_outNg, &n_outTotal, &t_outJenis, &n_outPcs, &t_outSubmit,
  &t_outClose,
  //====================PAGE NG====================//
  &pageNg,
  &b_ng1, &b_ng2, &b_ng3, &b_ng4, &b_ng5, &b_ng6, &b_ng7, &b_ng8, &b_ng9, &b_ng10, &b_ng11, &b_ng12, &b_ngClose,
  &t_ng1, &t_ng2, &t_ng3, &t_ng4, &t_ng5, &t_ng6, &t_ng7, &t_ng8, &t_ng9, &t_ng10, &t_ng11, &t_ng12, &t_ngClose,
  //====================PAGE DT LAYER====================//
  &pageDtLayer,
  &b_dtLayer1, &b_dtLayer2, &b_dtLayer3, &b_dtLayer4, &b_dtLayer5, &b_dtLayer6, &b_dtLayerRun, &b_dtLayerClose,
  &t_dtLayer1, &t_dtLayer2, &t_dtLayer3, &t_dtLayer4, &t_dtLayer5, &t_dtLayer6, &t_dtLayerRun, &t_dtLayerClose,
  //====================PAGE DT MC====================//
  &pageDtMc,
  &b_dtMc1, &b_dtMc2, &b_dtMc3, &b_dtMc4, &b_dtMc5, &b_dtMc6, &b_dtMc7, &b_dtMc8, &b_dtMc9, &b_dtMcRun, &b_dtMcClose,
  &t_dtMc1, &t_dtMc2, &t_dtMc3, &t_dtMc4, &t_dtMc5, &t_dtMc6, &t_dtMc7, &t_dtMc8, &t_dtMc9, &t_dtMcRun, &t_dtMcClose,
  //====================PAGE DT INFO====================//
  &pageDtInfo,
  &b_dtInfo, &b_dtInfoStart, &b_dtInfoSwitch,
  &t_dtInfo, &t_dtInfoStart, &t_dtInfoSwitch,
  //====================PAGE WARNING====================//
  &pageWarn,
  &b_warnBorder, &b_warnB1, &b_warnB2, &b_warnB3, &b_warnB4, &b_warnB5, &b_warnB6, &b_warnOk,
  &t_warnBorder, &t_warnB1, &t_warnB2, &t_warnB3, &t_warnB4, &t_warnB5, &t_warnB6, &t_warnOk,
  NULL
};

void Address(){
  for (int i = 0; i < 6; i++) {
    int pinValue = digitalRead(AddressPin[i]); // Read HIGH or LOW based on PIN
    bitWrite(address, i, pinValue); // Save as i byte 
  }
  full_mqtt_topic      = "SF-HPBP/" + String(address);
  mqtt_topic           = full_mqtt_topic.c_str();
   
  full_topic           = "BP/" + String(address);
  topic                = full_topic.c_str();
   
  esp_client           = "IoT_client" + String(mqtt_topic);
  control_mqtt_topic   = "CONTROL/" + String(mqtt_topic);
  read_mqtt_topic      = "READ/" + String(mqtt_topic);
  wifi_mqtt_topic      = "WIFI/" + String(mqtt_topic);
  ip_mqtt_topic        = "IP/" + String(mqtt_topic);
  dataA_mqtt_topic     = "dataA/" + String(mqtt_topic);
  dataB_mqtt_topic     = "dataB/" + String(mqtt_topic);
  revdataA_mqtt_topic  = "revdataA/" + String(mqtt_topic);
  revdataB_mqtt_topic  = "revdataB/" + String(mqtt_topic);
  cycle_mqtt_topic     = "cycle/" + String(mqtt_topic);
  idnama_mqtt_topic    = "idnama/" + String(mqtt_topic);
}

//Connecting ESP32 to WiFi
void setup_wifi() {
  delay(100);
  Serial.println("Connecting to WiFi...");
  WiFi.setHostname(full_mqtt_topic.c_str()); //definisi hostname
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  lastBlinkTime = millis();  
  ledState      = false;

  Serial.println("\nWiFi connected!");
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Current ESP32 IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway (router) IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Subnet Mask: " );
  Serial.println(WiFi.subnetMask());
  Serial.print("Primary DNS: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("Secondary DNS: ");
  Serial.println(WiFi.dnsIP(1));

  // Mulai server OTA
  httpUpdater.setup(&httpServer, "/update", ota_username, ota_password);
  httpServer.begin();
  Serial.println("OTA Web Server started.");
}

//RECONNECT
void reconnect() {
  if (client.connected()) return;
  if (millis() - lastReconnectAttempt < 2000) return;
  lastReconnectAttempt = millis();
  Serial.print("Connecting to MQTT...");
  if (client.connect(esp_client.c_str())) {
    Serial.println("connected!");
    client.subscribe(control_mqtt_topic.c_str());
    client.subscribe(revdataA_mqtt_topic.c_str());
    client.subscribe(revdataB_mqtt_topic.c_str());
    client.subscribe(cycle_mqtt_topic.c_str());
  } else {
    Serial.print("failed, rc=");
    Serial.println(client.state());
  }
}

//Interupt Counter
void IRAM_ATTR detectReset() {
  unsigned long now = millis();
  if (now - lastDebounceTime > debounceDelay) {
    if (!isScannerReady) {
      resetCount++;
      lastDebounceTime = now;
      shouldPublishCounter = true;
    }
  }
}

void buzzer() {
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
}

void longBuzz() {
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
}

void sendDataA() {
  if (!client.connected()) return;
  StaticJsonDocument<512> doc;
  doc["idfgs"]    = aIdfgs;
  doc["model"]    = aModel;
  doc["lotwo"]    = aLotWo;
  doc["lotwg"]    = aLotWg;
  doc["lotisi"]   = aLotIsi;
  doc["lotkuota"] = aLotKuota;
  doc["total"]    = aLotTotal;
  doc["noreg"]    = aNoreg;
  char bufferA[512];
  serializeJson(doc, bufferA);
  client.publish(dataA_mqtt_topic.c_str(), bufferA, true); // retained
}

void sendDataB() {
  if (!client.connected()) return;
  StaticJsonDocument<512> doc;
  doc["idfgs"]    = bIdfgs;
  doc["model"]    = bModel;
  doc["lotwo"]    = bLotWo;
  doc["lotwg"]    = bLotWg;
  doc["lotisi"]   = bLotIsi;
  doc["lotkuota"] = bLotKuota;
  doc["total"]    = bLotTotal;
  doc["noreg"]    = bNoreg;
  char bufferB[512];
  serializeJson(doc, bufferB);
  client.publish(dataB_mqtt_topic.c_str(), bufferB, true); 
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Wire.begin();
  setup_wifi();
  Address();
  for(int i=0;i<=5;i++){
    pinMode(AddressPin[i], INPUT);
    bitWrite(address,i,digitalRead(AddressPin[i]));
  }

  attachInterrupt(digitalPinToInterrupt(counterPin), detectReset, FALLING);

  String mac_addr = WiFi.macAddress();
  unsigned long start = millis();
  while (millis() - start < 2000) {
    client.loop();
    delay(1);
  }

  settingLayer();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  client.setKeepAlive(60);
  client.setSocketTimeout(5);

  //PAGE MAIN
  pageMain.attachPop(pageMainPop);
  b_aLayer.attachPop(b_aLayerRls);
  b_aModel.attachPop(b_aModelRls);
  b_aCav.attachPop(b_aCavRls);
  b_aIsi.attachPop(b_aIsiRls);
  b_aOutput.attachPop(b_aOutputRls);
  b_bLayer.attachPop(b_bLayerRls);
  b_bModel.attachPop(b_bModelRls);
  b_bCav.attachPop(b_bCavRls);
  b_bIsi.attachPop(b_bIsiRls);
  b_bOutput.attachPop(b_bOutputRls);
  b_iconSwitch.attachPop(b_iconSwitchRls);
  b_operator1.attachPop(b_operator1Rls);
  b_operator2.attachPop(b_operator2Rls);
  b_iconLogin1.attachPop(b_iconLogin1Rls);
  b_iconLogin2.attachPop(b_iconLogin2Rls);
  b_indikator.attachPop(b_indikatorRls);
  tm1.attachTimer(tm1TimerCallback);

  //PAGE KEY A
  keybdA.attachPop(keybdAPop);
  b_b0a.attachPop(b_b0aRls); b_b1a.attachPop(b_b1aRls); b_b2a.attachPop(b_b2aRls); b_b3a.attachPop(b_b3aRls); b_b4a.attachPop(b_b4aRls);
  b_b5a.attachPop(b_b5aRls); b_b6a.attachPop(b_b6aRls); b_b7a.attachPop(b_b7aRls); b_b8a.attachPop(b_b8aRls); b_b9a.attachPop(b_b9aRls);
  b_b20a.attachPop(b_b20aRls); b_b21a.attachPop(b_b21aRls); b_b22a.attachPop(b_b22aRls); b_b23a.attachPop(b_b23aRls); b_b24a.attachPop(b_b24aRls);
  b_b25a.attachPop(b_b25aRls); b_b26a.attachPop(b_b26aRls); b_b27a.attachPop(b_b27aRls); b_b28a.attachPop(b_b28aRls); b_b40a.attachPop(b_b40aRls);
  b_b41a.attachPop(b_b41aRls); b_b42a.attachPop(b_b42aRls); b_b43a.attachPop(b_b43aRls); b_b44a.attachPop(b_b44aRls); b_b45a.attachPop(b_b45aRls);
  b_b46a.attachPop(b_b46aRls); b_b200a.attachPop(b_b200aRls); b_b210a.attachPop(b_b210aRls); b_b220a.attachPop(b_b220aRls); b_b240a.attachPop(b_b240aRls);
  b_b251a.attachPop(b_b251aRls);

  //PAGE KEY B
  keybdB.attachPop(keybdBPop);
  b_b0.attachPop(b_b0Rls); b_b1.attachPop(b_b1Rls); b_b2.attachPop(b_b2Rls); b_b3.attachPop(b_b3Rls); b_b4.attachPop(b_b4Rls); b_b5.attachPop(b_b5Rls);
  b_b6.attachPop(b_b6Rls); b_b7.attachPop(b_b7Rls); b_b8.attachPop(b_b8Rls); b_b9.attachPop(b_b9Rls); b_b200.attachPop(b_b200Rls); b_b210.attachPop(b_b210Rls); b_b251.attachPop(b_b251Rls);
  
  //PAGE LOGIN
  pageLogin.attachPop(pageLoginPop);
  b_loginNoreg.attachPop(b_loginNoregRls);
  b_loginNama.attachPop(b_loginNamaRls);
  b_loginLogout.attachPop(b_loginLogoutRls);
  b_loginOk.attachPop(b_loginOkRls);
  b_loginClose.attachPop(b_loginCloseRls);

  //PAGE LOT
  pageLot.attachPop(pageLotPop);
  b_lotWo.attachPop(b_lotWoRls);
  b_lotWg.attachPop(b_lotWgRls);
  b_lotPcs.attachPop(b_lotPcsRls);
  b_lotKuota.attachPop(b_lotKuotaRls);
  b_lotIsi.attachPop(b_lotIsiRls);
  b_lotSubmit.attachPop(b_lotSubmitRls);
  b_lotOk.attachPop(b_lotOkRls);
  b_lotClose.attachPop(b_lotCloseRls);

  // dsb_lotCpA.attachPop(dsb_lotCpARls, &dsb_lotCpA);
  // dsb_lotCpB.attachPop(dsb_lotCpBRls, &dsb_lotCpB);
  // dsb_lotCpC.attachPop(dsb_lotCpCRls, &dsb_lotCpC);
  // dsb_lotCpD.attachPop(dsb_lotCpDRls, &dsb_lotCpD);
  // dsb_lotCpE.attachPop(dsb_lotCpERls, &dsb_lotCpE);

  //PAGE MOULDING
  pageMld.attachPop(pageMldPop);
  b_mldKode.attachPop(b_mldKodeRls);
  b_mldModel.attachPop(b_mldModelRls);
  b_mldCav.attachPop(b_mldCavRls);
  b_mldIsi.attachPop(b_mldIsiRls);
  b_mldClear.attachPop(b_mldClearRls);
  b_mldOk.attachPop(b_mldOkRls);
  b_mldClose.attachPop(b_mldCloseRls);

  //PAGE OUTPUT
  pageOutput.attachPop(pageOutputPop);
  b_outOk.attachPop(b_outOkRls);
  b_outNg.attachPop(b_outNgRls);
  b_outTotal.attachPop(b_outTotalRls);
  b_outJenis.attachPop(b_outJenisRls);
  b_outPcs.attachPop(b_outPcsRls);
  b_outSubmit.attachPop(b_outSubmitRls);
  b_outClose.attachPop(b_outCloseRls);

  //PAGE NG
  pageNg.attachPop(pageNgPop);
  b_ng1.attachPop(b_ng1Rls);
  b_ng2.attachPop(b_ng2Rls);
  b_ng3.attachPop(b_ng3Rls);
  b_ng4.attachPop(b_ng4Rls);
  b_ng5.attachPop(b_ng5Rls);
  b_ng6.attachPop(b_ng6Rls);
  b_ng7.attachPop(b_ng7Rls);
  b_ng8.attachPop(b_ng8Rls);
  b_ng9.attachPop(b_ng9Rls);
  b_ng10.attachPop(b_ng10Rls);
  b_ng11.attachPop(b_ng11Rls);
  b_ng12.attachPop(b_ng12Rls);
  b_ngClose.attachPop(b_ngCloseRls);

  //PAGE DT LAYER
  pageDtLayer.attachPop(pageDtLayerPop);
  b_dtLayer1.attachPop(b_dtLayer1Rls);
  b_dtLayer2.attachPop(b_dtLayer2Rls);
  b_dtLayer3.attachPop(b_dtLayer3Rls);
  b_dtLayer4.attachPop(b_dtLayer4Rls);
  b_dtLayer5.attachPop(b_dtLayer5Rls);
  b_dtLayer6.attachPop(b_dtLayer6Rls);
  b_dtLayerRun.attachPop(b_dtLayerRunRls);
  b_dtLayerClose.attachPop(b_dtLayerCloseRls);

  //====================PAGE DT MC====================//
  pageDtMc.attachPop(pageDtMcPop);
  b_dtMc1.attachPop(b_dtMc1Rls);
  b_dtMc2.attachPop(b_dtMc2Rls);
  b_dtMc3.attachPop(b_dtMc3Rls);
  b_dtMc4.attachPop(b_dtMc4Rls);
  b_dtMc5.attachPop(b_dtMc5Rls);
  b_dtMc6.attachPop(b_dtMc6Rls);
  b_dtMc7.attachPop(b_dtMc7Rls);
  b_dtMc8.attachPop(b_dtMc8Rls);
  b_dtMc9.attachPop(b_dtMc9Rls);
  b_dtMcRun.attachPop(b_dtMcRunRls);
  b_dtMcClose.attachPop(b_dtMcCloseRls);

  //====================PAGE DT INFO====================//
  pageDtInfo.attachPop(pageDtInfoPop);
  b_dtInfo.attachPop(b_dtInfoRls);
  b_dtInfoStart.attachPop(b_dtInfoStartRls);
  b_dtInfoSwitch.attachPop(b_dtInfoSwitchRls);

  //====================PAGE DT INFO====================//
  pageWarn.attachPop(pageWarnPop);
  b_warnOk.attachPop(b_warnOkRls);

  pageMain.show();
}

void loop() {
  client.loop();
  httpServer.handleClient();
  rtc.refresh();
  nexLoop(nex_listen_list);
  digitalWrite(ioPin[2], flagStart ? LOW : HIGH);

  //HANDLE SCANNER
   if (Serial2.available()) {
    String scanData = Serial2.readStringUntil('\n');
    scanData.trim();
    if (scanData.length() > 0) {
      // SCAN NOREG
      if (scanNoregMode) {
        scanData.toCharArray(
          input_loginNoreg,
          sizeof(input_loginNoreg)
        );
        t_loginNoreg.setText(input_loginNoreg);
        Serial.print("NOREG Scan : ");
        Serial.println(input_loginNoreg);
      }

      // SCAN LOT
      if (scanLotMode) {
        scanData.toCharArray(
          input_lotWo,
          sizeof(input_lotWo)
        );
        t_lotWo.setText(input_lotWo);
        Serial.print("LOT Scan : ");
        Serial.println(input_lotWo);
      }
    }
  }
}

void callback (char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<512> doc;
  DeserializationError err = deserializeJson(doc, payload, length);
  if (err) {
    Serial.println("JSON Parse Error");
    return;
  }
  String tpc = String(topic);
  /* ===================== A ===================== */
  if (tpc == revdataA_mqtt_topic.c_str()) {
    strlcpy(aIdfgs, doc["idfgs"] | "", sizeof(aIdfgs));
    aIdfgs_length = strlen(aIdfgs);
    strlcpy(aModel, doc["model"] | "", sizeof(aModel));
    aModel_length = strlen(aModel);
    strlcpy(aLotWo, doc["lotwo"] | "", sizeof(aLotWo));
    aLotWo_length = strlen(aLotWo);
    strlcpy(aLotWg, doc["lotwg"] | "", sizeof(aLotWg));
    aLotWg_length = strlen(aLotWg);
    strlcpy(aNoreg, doc["noreg"] | "", sizeof(aNoreg));
    aLotIsi       = doc["lotisi"]   | 0;
    aLotKuota     = doc["lotkuota"] | 0;
    aLotTotal     = doc["total"]    | 0;
    aMldIsi       = aLotIsi;
    t_aModel.setText(aModel);
    n_aIsi.setValue(aLotIsi);
    n_aOutput.setValue(aLotTotal);
    Serial.println("DATA A UPDATED FROM DB");
  }

  /* ===================== B ===================== */
  else if (tpc == revdataB_mqtt_topic.c_str()) {
    strlcpy(bIdfgs, doc["idfgs"] | "", sizeof(bIdfgs));
    bIdfgs_length = strlen(bIdfgs);
    strlcpy(bModel, doc["model"] | "", sizeof(bModel));
    bModel_length = strlen(bModel);
    strlcpy(bLotWo, doc["lotwo"] | "", sizeof(bLotWo));
    bLotWo_length = strlen(bLotWo);
    strlcpy(bLotWg, doc["lotwg"] | "", sizeof(bLotWg));
    bLotWg_length = strlen(bLotWg);
    strlcpy(bNoreg, doc["noreg"] | "", sizeof(bNoreg));
    bLotIsi       = doc["lotisi"]   | 0;
    bLotKuota     = doc["lotkuota"] | 0;
    bLotTotal     = doc["total"]    | 0;
    bMldIsi       = bLotIsi;
    t_bModel.setText(bModel);
    n_bIsi.setValue(bLotIsi);
    n_bOutput.setValue(bLotTotal);

    Serial.println("DATA B UPDATED FROM DB");
  }
}

