#include <stdio.h>

RTC_DS3231 rtc;
bool rtcReady = false;
unsigned long lastRtcUpdate = 0;
const unsigned long RTC_UPDATE_INTERVAL = 1000;

void sendRtcText(const char* value) {
  char command[96];
  snprintf(command, sizeof(command), "pageSys.tNow.txt=\"%s\"", value);
  sendCommand(command);
}

void initRtc() {
  Wire.begin();

  if (!rtc.begin()) {
    rtcReady = false;
    return;
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtcReady = true;
}

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
    now.second()
  );
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
}
