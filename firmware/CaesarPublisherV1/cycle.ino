bool frontLatched = false;
bool backLatched = false;
bool upLatched = false;
bool downLatched = false;
bool cycleStarted = false;
bool expectUp = true;
uint8_t stepCount = 0;
char cycleStartTime[32];

void initCycleInputs() {
  frontLatched = digitalRead(FRONT_PIN) == LOW;
  backLatched = digitalRead(BACK_PIN) == LOW;
  upLatched = digitalRead(UP_PIN) == LOW;
  downLatched = digitalRead(DOWN_PIN) == LOW;
}

void publishStart() {
  if (!mqttClient.connected()) {
    return;
  }

  mqttClient.publish(START_TOPIC, "START", false);
}

void publishCycle() {
  char finishTime[32];
  char payload[192];
  if (!mqttClient.connected() || !getTimestamp(finishTime, sizeof(finishTime))) {
    return;
  }

  snprintf(payload, sizeof(payload), "{\"event\":\"1cycle\",\"startTime\":\"%s\",\"finishTime\":\"%s\"}", cycleStartTime, finishTime);
  mqttClient.publish(FINISH_TOPIC, payload, false);
}

void startCycle() {
  if (cycleStarted || !getTimestamp(cycleStartTime, sizeof(cycleStartTime))) {
    return;
  }

  cycleStarted = true;
  stepCount = 0;
  expectUp = true;
  publishStart();
}

void finishCycle() {
  publishCycle();
  cycleStarted = false;
  stepCount = 0;
  expectUp = true;
}

void updateCycleInputs() {
  bool frontPressed = digitalRead(FRONT_PIN) == LOW;
  bool backPressed = digitalRead(BACK_PIN) == LOW;
  bool upPressed = digitalRead(UP_PIN) == LOW;
  bool downPressed = digitalRead(DOWN_PIN) == LOW;

  if (frontPressed && !frontLatched) {
    startCycle();
  }
  if (backPressed && !backLatched) {
    startCycle();
  }

  if (upPressed && !upLatched && expectUp && cycleStarted) {
    stepCount++;
    expectUp = false;
  }
  if (downPressed && !downLatched && !expectUp && cycleStarted) {
    stepCount++;
    expectUp = true;
  }

  frontLatched = frontPressed;
  backLatched = backPressed;
  upLatched = upPressed;
  downLatched = downPressed;

  if (cycleStarted && stepCount >= 5) {
    finishCycle();
  }
}
