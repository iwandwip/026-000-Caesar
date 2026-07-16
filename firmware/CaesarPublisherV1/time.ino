bool ntpConfigured = false;
bool ntpReady = false;
unsigned long lastNtpAttempt = 0;
const unsigned long NTP_RETRY_INTERVAL = 1000;

void initTime() {
  setenv("TZ", WIB_TIMEZONE, 1);
  tzset();
}

void syncTime() {
  if (ntpReady || WiFi.status() != WL_CONNECTED || millis() - lastNtpAttempt < NTP_RETRY_INTERVAL) {
    return;
  }

  lastNtpAttempt = millis();
  if (!ntpConfigured) {
    configTzTime(WIB_TIMEZONE, NTP_SERVER);
    ntpConfigured = true;
  }

  struct tm timeInfo;
  ntpReady = getLocalTime(&timeInfo, 10);
}

bool getTimestamp(char* buffer, size_t length) {
  if (!ntpReady) {
    return false;
  }

  struct tm timeInfo;
  if (!getLocalTime(&timeInfo, 10)) {
    return false;
  }

  snprintf(
    buffer,
    length,
    "%04d-%02d-%02dT%02d:%02d:%02d.000+07:00",
    timeInfo.tm_year + 1900,
    timeInfo.tm_mon + 1,
    timeInfo.tm_mday,
    timeInfo.tm_hour,
    timeInfo.tm_min,
    timeInfo.tm_sec);
  return true;
}
