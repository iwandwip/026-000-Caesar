#include <stdio.h>

#if TIME_SOURCE_NTP
#include <time.h>
#include <WiFi.h>
#else
#include <Wire.h>
#include <RTClib.h>
#endif

#if TIME_SOURCE_RTC
RTC_DS3231 rtc;
bool rtcReady = false;
#endif

#if TIME_SOURCE_NTP
const unsigned long NTP_RETRY_INTERVAL = 1000;
bool ntpConfigured = false;
bool ntpReady = false;
unsigned long lastNtpAttempt = 0;
#endif

unsigned long lastRtcUpdate = 0;
const unsigned long RTC_UPDATE_INTERVAL = 1000;

void sendRtcText(const char* value) {
  char command[96];
  snprintf(command, sizeof(command), "pageSys.tNow.txt=\"%s\"", value);
  sendCommand(command);
}

void initTimeSource() {
#if TIME_SOURCE_NTP
  setenv("TZ", WIB_TIMEZONE, 1);
  tzset();
#else
  Wire.begin();
  if (!rtc.begin()) {
    rtcReady = false;
    return;
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtcReady = true;
#endif
}

#if TIME_SOURCE_NTP
void syncTimeSource() {
  if (ntpReady || WiFi.status() != WL_CONNECTED) {
    return;
  }

  if (millis() - lastNtpAttempt < NTP_RETRY_INTERVAL) {
    return;
  }
  lastNtpAttempt = millis();

  if (!ntpConfigured) {
    configTime(0, 0, NTP_SERVER);
    ntpConfigured = true;
  }

  struct tm timeInfo;
  if (getLocalTime(&timeInfo, 10)) {
    ntpReady = true;
  }
}
#else
void syncTimeSource() {}
#endif

void getCurrentTimestamp(char* buffer, size_t length) {
#if TIME_SOURCE_NTP
  if (!ntpReady) {
    snprintf(buffer, length, "0000-00-00 00:00:00");
    return;
  }

  struct tm timeInfo;
  if (!getLocalTime(&timeInfo, 10)) {
    snprintf(buffer, length, "0000-00-00 00:00:00");
    return;
  }

  snprintf(
    buffer,
    length,
    "%04d-%02d-%02d %02d:%02d:%02d",
    timeInfo.tm_year + 1900,
    timeInfo.tm_mon + 1,
    timeInfo.tm_mday,
    timeInfo.tm_hour,
    timeInfo.tm_min,
    timeInfo.tm_sec);
#else
  if (!rtcReady) {
    snprintf(buffer, length, "0000-00-00 00:00:00");
    return;
  }

  DateTime now = rtc.now();
  snprintf(
    buffer,
    length,
    "%04d-%02d-%02d %02d:%02d:%02d",
    now.year(),
    now.month(),
    now.day(),
    now.hour(),
    now.minute(),
    now.second());
#endif
}

void syncDashboardClock() {
#if TIME_SOURCE_NTP
  if (!ntpReady) {
    return;
  }

  struct tm timeInfo;
  if (!getLocalTime(&timeInfo, 10)) {
    return;
  }

  char hour[3];
  char minute[3];
  char second[3];
  snprintf(hour, sizeof(hour), "%02u", timeInfo.tm_hour);
  snprintf(minute, sizeof(minute), "%02u", timeInfo.tm_min);
  snprintf(second, sizeof(second), "%02u", timeInfo.tm_sec);

  sendInputValue("pageSys.nHH", timeInfo.tm_hour);
  sendInputValue("pageSys.nMM", timeInfo.tm_min);
  sendInputValue("pageSys.nSS", timeInfo.tm_sec);
  sendInputText("pageDashboard.tHH", hour);
  sendInputText("pageDashboard.tMM", minute);
  sendInputText("pageDashboard.tSS", second);
#else
  if (!rtcReady) {
    return;
  }

  DateTime now = rtc.now();
  char hour[3];
  char minute[3];
  char second[3];
  snprintf(hour, sizeof(hour), "%02u", now.hour());
  snprintf(minute, sizeof(minute), "%02u", now.minute());
  snprintf(second, sizeof(second), "%02u", now.second());

  sendInputValue("pageSys.nHH", now.hour());
  sendInputValue("pageSys.nMM", now.minute());
  sendInputValue("pageSys.nSS", now.second());
  sendInputText("pageDashboard.tHH", hour);
  sendInputText("pageDashboard.tMM", minute);
  sendInputText("pageDashboard.tSS", second);
#endif
}

void updateTnow() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastRtcUpdate < RTC_UPDATE_INTERVAL) {
    return;
  }

  lastRtcUpdate = currentMillis;

  char timestamp[25];
  getCurrentTimestamp(timestamp, sizeof(timestamp));
  sendRtcText(timestamp);
  syncDashboardClock();
}
