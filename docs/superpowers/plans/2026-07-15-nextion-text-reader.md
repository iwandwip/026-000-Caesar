# Nextion Text Reader Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Reliably read Text component values for mould and lot validation on ESP32.

**Architecture:** Add one local raw-protocol reader to `input.ino`. It sends `get <component>.txt`, consumes Nextion string response frames, and exposes a bounded null-terminated value to existing validation handlers. Existing lookup and page update logic remains unchanged.

**Tech Stack:** Arduino ESP32, ITEADLIB Arduino Nextion, Nextion serial protocol.

## Global Constraints

- Do not modify HMI files, scanner code, login code, or ITEADLIB.
- Preserve support for alphanumeric mould and lot codes.
- Only the four failing text reads use raw protocol helper.
- Nextion text success frame is `0x70 <text bytes> 0xFF 0xFF 0xFF`.
- Helper must bound writes, null-terminate output, and fail on timeout, malformed frames, or empty values.

---

### Task 1: Add Bounded Raw Text Reader

**Files:**
- Modify: `firmware/CaesarFirmwareV1/input.ino:3-13`
- Test: Hardware or HMI simulator using `tInMF`

**Interfaces:**
- Consumes: `sendCommand(const char*)`, `nexSerial`, Arduino `millis()`.
- Produces: `bool readNextionText(const char* component, char* buffer, size_t bufferSize)`.

- [ ] **Step 1: Establish failing behavior**

Enter `1111` in front mould input and press OK. Expected current behavior: `MOULD NG`, because `tInMF.getText()` returns empty.

- [ ] **Step 2: Add raw reader below `sendInputValue()`**

```cpp
bool readNextionText(const char* component, char* buffer, size_t bufferSize) {
  if (bufferSize == 0) {
    return false;
  }

  buffer[0] = '\0';

  char command[64];
  snprintf(command, sizeof(command), "get %s.txt", component);

  while (nexSerial.available()) {
    nexSerial.read();
  }

  sendCommand(command);

  uint16_t index = 0;
  uint8_t terminatorCount = 0;
  bool receivedHeader = false;
  unsigned long start = millis();

  while (millis() - start < 200) {
    if (!nexSerial.available()) {
      continue;
    }

    uint8_t value = nexSerial.read();
    if (!receivedHeader) {
      if (value == 0x70) {
        receivedHeader = true;
      }
      continue;
    }

    if (value == 0xFF) {
      terminatorCount++;
      if (terminatorCount == 3) {
        buffer[index] = '\0';
        return index > 0;
      }
      continue;
    }

    while (terminatorCount > 0) {
      if (index < bufferSize - 1) {
        buffer[index++] = 0xFF;
      }
      terminatorCount--;
    }

    if (index < bufferSize - 1) {
      buffer[index++] = value;
    }
  }

  buffer[0] = '\0';
  return false;
}
```

- [ ] **Step 3: Compile firmware when Arduino CLI exists**

Run: `arduino-cli compile --fqbn esp32:esp32:esp32 firmware/CaesarFirmwareV1`

Expected: build succeeds. If `arduino-cli` remains unavailable, record its absence and use hardware validation.

- [ ] **Step 4: Commit reader**

```bash
git add firmware/CaesarFirmwareV1/input.ino
git commit -m "fix(nextion): read text input frames directly"
```
```

### Task 2: Route Mould And Lot Validation Through Reader

**Files:**
- Modify: `firmware/CaesarFirmwareV1/input.ino:39-126`
- Test: Hardware or HMI simulator with valid and invalid mould/lot codes.

**Interfaces:**
- Consumes: `readNextionText(const char*, char*, size_t)` from Task 1.
- Produces: Existing handlers use actual Text input values before configured lookup.

- [ ] **Step 1: Replace four library reads**

```cpp
void handleFrontMould() {
  char code[32];
  readNextionText("tInMF", code, sizeof(code));
  const Mould* mould = findMould(code);
```

```cpp
void handleBackMould() {
  char code[32];
  readNextionText("tInMB", code, sizeof(code));
  const Mould* mould = findMould(code);
```

```cpp
void handleFrontLot() {
  char code[32];
  readNextionText("tInF", code, sizeof(code));
  const Lot* lot = findLot(code);
```

```cpp
void handleBackLot() {
  char code[32];
  readNextionText("tInB", code, sizeof(code));
  const Lot* lot = findLot(code);
```

- [ ] **Step 2: Run static diff validation**

Run: `git diff --check`

Expected: no output and exit code 0.

- [ ] **Step 3: Verify valid paths on HMI**

1. On front mould page, enter or scan `1111`, press OK; confirm model, cavity, and isi populate.
2. Repeat front lot page; confirm model, target, and quota populate.
3. Repeat both on back pages.

Expected: no valid input produces `MOULD NG` or `LOT NG`.

- [ ] **Step 4: Verify invalid path on HMI**

Enter `9999` on a mould page then a lot page, press OK.

Expected: existing `MOULD NG` and `LOT NG` labels appear; no system values persist from unknown code.

- [ ] **Step 5: Commit integration**

```bash
git add firmware/CaesarFirmwareV1/input.ino
git commit -m "fix(input): validate mould and lot text values"
```
```
