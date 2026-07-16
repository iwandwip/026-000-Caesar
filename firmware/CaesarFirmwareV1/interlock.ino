#define INTERLOCK_PIN 25

void initInterlock() {
  pinMode(INTERLOCK_PIN, OUTPUT);
  digitalWrite(INTERLOCK_PIN, LOW);
}

void setInterlock(bool ready) {
  digitalWrite(INTERLOCK_PIN, ready ? HIGH : LOW);
}
