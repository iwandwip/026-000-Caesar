#include <stdio.h>

#if TIME_SOURCE_NTP
#include <time.h>
#include <WiFi.h>
#endif

RTC_DS3231 rtc;
bool rtcReady = false;
unsigned long lastRtcUpdate = 0;
const unsigned long RTC_UPDATE_INTERVAL = 1000;

#if TIME_SOURCE_NTP
const unsigned long NTP_RETRY_INTERVAL = 1000;
bool rtcNtpConfigured = false;
bool rtcNtpSynchronized = false;
unsigned long lastNtpAttempt = 0;
#endif

void sendRtcText(const char* value) {
  char command[96];
  snprintf(command, sizeof(command), "pageSys.tNow.txt=\"%s\"", value);
  sendCommand(command);
}

void initRtc() {
  Wire.begin();

#if TIME_SOURCE_NTP
  setenv("TZ", WIB_TIMEZONE, 1);
  tzset();
#endif

  if (!rtc.begin()) {
    rtcReady = false;
    return;
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtcReady = true;
}

#if TIME_SOURCE_NTP
void syncRtcWithNtp() {
  if (!rtcReady || rtcNtpSynchronized || WiFi.status() != WL_CONNECTED) {
    return;
  }

  if (millis() - lastNtpAttempt < NTP_RETRY_INTERVAL) {
    return;
  }
  lastNtpAttempt = millis();

  if (!rtcNtpConfigured) {
    configTime(0, 0, NTP_SERVER);
    rtcNtpConfigured = true;
  }

  struct tm timeInfo;
  if (getLocalTime(&timeInfo, 10)) {
    rtc.adjust(DateTime(
      timeInfo.tm_year + 1900,
      timeInfo.tm_mon + 1,
      timeInfo.tm_mday,
      timeInfo.tm_hour,
      timeInfo.tm_min,
      timeInfo.tm_sec));
    rtcNtpSynchronized = true;
  }
}
#else
void syncRtcWithNtp() {}
#endif

void getRtcTimestamp(char* buffer, size_t length) {
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
}

void syncDashboardClock() {
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
}

void updateTnow() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastRtcUpdate < RTC_UPDATE_INTERVAL) {
    return;
  }

  lastRtcUpdate = currentMillis;

  char timestamp[25];
  getRtcTimestamp(timestamp, sizeof(timestamp));
  sendRtcText(timestamp);
  syncDashboardClock();
}
