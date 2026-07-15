# Nextion Text Reader Design

## Goal

Read mould and lot text inputs reliably on ESP32. `NexText::getText()` currently returns an empty string, causing valid codes to display `MOULD NG` or `LOT NG`.

## Scope

Add one firmware-local helper in `firmware/CaesarFirmwareV1/input.ino`. Replace four failing `NexText::getText()` calls:

- `tInMF` in front mould validation
- `tInMB` in back mould validation
- `tInLotF` in front lot validation
- `tInLotB` in back lot validation

No HMI, scanner, login, or ITEADLIB changes.

## Design

`readNextionText(component, buffer, bufferSize)` sends `get <component>.txt` through `sendCommand()`. It reads from `nexSerial` until timeout and accepts only a Nextion string response:

```
0x70 <text bytes> 0xFF 0xFF 0xFF
```

The helper excludes byte `0x70` and response terminator from output. It null-terminates output, bounds writes to `bufferSize - 1`, and returns `true` only after complete string response. Timeout, malformed response, or an empty string return `false`.

Before command, helper drains stale Nextion serial bytes. It runs only from OK-button callbacks after `nexLoop()` has dispatched touch event, so command response owns serial stream.

## Call Flow

1. User types or scans code into existing Text input.
2. User presses OK.
3. Callback invokes `readNextionText()` for field component name.
4. On read failure, existing lookup receives empty string and preserves existing `MOULD NG` or `LOT NG` behavior.
5. On valid read, existing `findMould()` or `findLot()` lookup and page-system updates remain unchanged.

## Error Handling

- Timeout: output is empty; validation reports existing NG status.
- Empty Nextion text: output is empty; validation reports existing NG status.
- Oversized text: helper truncates safely, drains response terminator, then returns completed response. Lookup only succeeds if truncated value matches a configured code.
- Non-string response: discard bytes and return failure after timeout.

## Verification

- Run `git diff --check`.
- Compile if Arduino CLI becomes available.
- On hardware/HMI simulator, enter or scan `1111` on front/back mould and lot pages, press OK, and confirm existing model/cavity/target fields populate.
- Confirm an unknown code still shows `MOULD NG` or `LOT NG`.
