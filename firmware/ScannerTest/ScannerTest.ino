const uint8_t SCANNER_RX_PIN = 16;
const uint8_t SCANNER_TX_PIN = 17;
const uint32_t SCANNER_BAUD = 9600;
const uint32_t DEBUG_BAUD = 115200;

const uint8_t FIND_BAUD_COMMAND[] = { 0x7E, 0x00, 0x07, 0x01, 0x00, 0x2A, 0x02, 0xD8, 0x0F };

char scanBuffer[128];
uint8_t scanIndex = 0;
uint32_t scanCount = 0;
uint32_t firstCharAt = 0;
uint32_t lastCharAt = 0;

void printByteHex(uint8_t value) {
  if (value < 0x10) {
    Serial.print('0');
  }
  Serial.print(value, HEX);
}

void printScanLine() {
  scanBuffer[scanIndex] = '\0';
  scanCount++;

  Serial.print("scan #");
  Serial.print(scanCount);
  Serial.print(" chars=");
  Serial.print(scanIndex);
  Serial.print(" duration_ms=");
  Serial.print(lastCharAt - firstCharAt);
  Serial.print(" data=");
  Serial.println(scanBuffer);
}

void resetScanBuffer() {
  scanIndex = 0;
  firstCharAt = 0;
  lastCharAt = 0;
}

void handleScannerByte(uint8_t value) {
  uint32_t now = millis();

  Serial.print("byte t=");
  Serial.print(now);
  Serial.print(" hex=0x");
  printByteHex(value);
  Serial.print(" char=");

  if (value >= 32 && value <= 126) {
    Serial.write(value);
  } else {
    Serial.print('.');
  }

  if (lastCharAt > 0) {
    Serial.print(" gap_ms=");
    Serial.print(now - lastCharAt);
  }

  Serial.println();

  if (value == '\n' || value == '\r') {
    if (scanIndex > 0) {
      lastCharAt = now;
      printScanLine();
      resetScanBuffer();
    }
    return;
  }

  if (scanIndex == 0) {
    firstCharAt = now;
  }

  lastCharAt = now;

  if (scanIndex < sizeof(scanBuffer) - 1) {
    scanBuffer[scanIndex] = (char)value;
    scanIndex++;
  } else {
    Serial.println("buffer full, reset");
    resetScanBuffer();
  }
}

void sendFindBaudCommand() {
  Serial.println("send find baud command");
  Serial2.write(FIND_BAUD_COMMAND, sizeof(FIND_BAUD_COMMAND));
}

void setup() {
  Serial.begin(DEBUG_BAUD);
  delay(1000);

  Serial.println();
  Serial.println("ScannerTest GM66");
  Serial.println("debug=Serial 115200");
  Serial.println("scanner=Serial2 9600 8N1 RX=16 TX=17");
  Serial.println("scan barcode now");

  Serial2.begin(SCANNER_BAUD, SERIAL_8N1, SCANNER_RX_PIN, SCANNER_TX_PIN);
  delay(300);
  sendFindBaudCommand();
}

void loop() {
  while (Serial2.available()) {
    handleScannerByte((uint8_t)Serial2.read());
  }

  if (Serial.available()) {
    char command = (char)Serial.read();
    if (command == 'b' || command == 'B') {
      sendFindBaudCommand();
    }
  }
}
