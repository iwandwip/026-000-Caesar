# Caesar ESP32 Firmware

`CaesarFirmwareV1` runs on ESP32 and controls Nextion process state, a GM66 scanner, and a DS3231 RTC. The firmware validates all operator, mould, lot, and downtime actions. The HMI displays values and reports page or button events.

Open `firmware/CaesarFirmwareV1/CaesarFirmwareV1.ino` in Arduino IDE or your ESP32 Arduino build environment.

## Source Files

| File | Responsibility |
| --- | --- |
| `CaesarFirmwareV1.ino` | Page and component declarations, setup, main loop, and callback registration. |
| `config.h` | Serial settings and static operator, mould, and lot lookup tables. |
| `login.ino` | Front and back operator login, logout, and page tracking. |
| `input.ino` | Mould and lot input, validation, clearing, and raw Nextion value reads. |
| `scanner.ino` | GM66 receive buffer and page-specific scanner routing. |
| `downtime.ino` | Front, back, and machine downtime state transitions. |
| `rtc.ino` | DS3231 initialization and one-second HMI time updates. |

## Dependencies

The repository includes local Arduino libraries under `firmware/libraries/`.

- ESP32 Arduino core
- ITEADLIB Arduino Nextion library
- Adafruit RTClib
- Adafruit BusIO

Select the ESP32 board that matches the deployed hardware. The repository does not contain an Arduino CLI configuration or a pinned board package version.

## Wiring and Serial Ports

### Nextion HMI

The ITEAD library uses ESP32 `Serial` for Nextion traffic:

```cpp
// firmware/libraries/ITEADLIB_Arduino_Nextion/NexConfig.h
#define nexSerial Serial
```

Nextion Editor Debug and the deployed display share this serial channel. Do not open Arduino Serial Monitor on the same USB serial port while Nextion Editor Debug is connected. ITEAD debug output stays disabled because it would corrupt Nextion protocol frames.

### GM66 Scanner

`config.h` configures the scanner on ESP32 `Serial2`.

```cpp
#define SCANNER_SERIAL Serial2
#define SCANNER_RX_PIN 16
#define SCANNER_TX_PIN 17
#define SCANNER_BAUD 9600
```

| GM66 wire | ESP32 connection |
| --- | --- |
| TX | GPIO16, Serial2 RX |
| RX | GPIO17, Serial2 TX |
| VCC | 5V |
| GND | GND |

The scanner uses `9600`, `SERIAL_8N1`.

### DS3231 RTC

`rtc.ino` uses `Wire.begin()`, which uses ESP32 default I2C pins unless the board setup overrides them.

| DS3231 signal | Default ESP32 pin |
| --- | --- |
| SDA | GPIO21 |
| SCL | GPIO22 |

The firmware reads time once each second and writes `YYYY-MM-DD HH:MM:SS` to `pageSys.tNow.txt`. If the RTC lost power, firmware initializes it with compile time. Set the RTC to production time before deployment.

## Nextion Page IDs

Page IDs follow page order in Nextion Editor. Firmware declarations must match this mapping.

| ID | Page |
| --- | --- |
| 0 | `pageDashboard` |
| 1 | `pageLoginF` |
| 2 | `pageLoginB` |
| 3 | `pageMldF` |
| 4 | `pageMldB` |
| 5 | `pageLotF` |
| 6 | `pageLotB` |
| 7 | `pageOutputNG` |
| 8 | `pageInputNGF` |
| 9 | `pageInputNGB` |
| 10 | `pageDtF` |
| 11 | `pageDtB` |
| 12 | `pageDtInfoF` |
| 13 | `pageDtInfoB` |
| 14 | `pageDtMc` |
| 15 | `pageDtMcInfo` |
| 16 | `pageSim` |
| 17 | `pageSys` |
| 18 | `keybdA` |
| 19 | `keybdB` |

Each page sends a page notification through its Preinitialize event. Example for the dashboard:

```text
printh 65 00 00 00 FF FF FF
```

Use the page ID in the third byte. `pageCallback()` updates `currentPageId`, which controls where scanner data is written.

## Process Data

`config.h` contains the current static lookup tables.

| Code | Operator | Mould | Cavity | Lot model | Target |
| --- | --- | --- | ---: | --- | ---: |
| `1111` | Budi Andianto | MOULD A | 8 | DEP FUSO | 108 |
| `2222` | Andi Saputra | MOULD B | 6 | VARIO125 | 120 |
| `3333` | Siti Nurhaliza | MOULD C | 4 | GENIO | 96 |

Change these tables in `config.h` when production master data changes.

## Login Flow

1. Open `pageLoginF` or `pageLoginB`.
2. Scan an operator barcode or enter an ID manually.
3. Press OK.
4. Firmware validates `OPERATORS[]`, writes the operator data to `pageSys`, and displays the operator name.

Invalid IDs display `ID SALAH`.

## Mould and Lot Flow

Scanner and manual keyboard input follow the same model: fill a field, then press OK. Firmware owns the validation and save action.

### Mould

On `pageMldF` and `pageMldB`, a valid code writes the mould code, model, cavity, and default isi values to `pageSys`. Firmware also initializes `pageLotF.nIsiLotF` or `pageLotB.nIsiLotB` from the cavity count.

### Lot

On `pageLotF` and `pageLotB`, a valid lot requires a mould on the same side. Firmware reads cavity and isi through raw Nextion number responses, then rejects these conditions:

| Condition | HMI message |
| --- | --- |
| Unknown lot code | `LOT NG` |
| Cavity read failure | `CAV READ NG` |
| Isi read failure | `ISI READ NG` |
| No validated mould | `NO MOULD` |
| Isi is zero | `ISI 0` |
| Isi exceeds cavity | `ISI > CAV` |

Keyboard edits to lot isi remain in the lot page. Returning from a keyboard page does not overwrite the value.

`readNextionText()` and `readNextionValue()` in `input.ino` read `0x70` text and `0x71` numeric response frames directly. They avoid the ITEAD library response parser when delayed serial frames would produce an empty or stale result.

## Scanner Routing

Scanner input routes by `currentPageId`:

| Page | Scanner action |
| --- | --- |
| `pageLoginF` | Writes front operator ID. |
| `pageLoginB` | Writes back operator ID. |
| `pageMldF` | Fills front mould code. |
| `pageMldB` | Fills back mould code. |
| `pageLotF` | Fills front lot code and loads cavity as default isi. |
| `pageLotB` | Fills back lot code and loads cavity as default isi. |

The scanner does not auto-confirm a value. The operator presses OK after each scan.

## Downtime Flow

Downtime buttons update `pageSys`, lock machine readiness, and show a matching information page.

| Selection page | Shared state | Information page |
| --- | --- | --- |
| `pageDtF` | `nFDtAct`, `tFDtType`, `tFDtStart` | `pageDtInfoF` |
| `pageDtB` | `nBDtAct`, `tBDtType`, `tBDtStart` | `pageDtInfoB` |
| `pageDtMc` | `nMDtAct`, `tMDtType`, `tMDtStart` | `pageDtMcInfo` |

After a reason is selected, firmware uses this route:

```text
pageDtF / pageDtB / pageDtMc
  -> pageDashboard
  -> matching downtime information page
```

`showDowntimeInfo()` waits 100 ms after loading `pageDashboard`, then opens the matching information page. The info page Postinitialize event copies downtime type and start time from `pageSys`.

RUNNING clears the matching downtime state and returns to `pageDashboard`.

## HMI Integration Rules

- Keep `Send Component ID: on release` enabled for firmware-managed buttons.
- Keep the page Preinitialize notifications aligned with the page ID table.
- Remove inline Touch Release logic from firmware-managed mould, lot, and downtime buttons. Firmware callbacks own these transitions.
- Keep Back button behavior only where it remains HMI navigation. Firmware handles state changes and validation.
- Do not add lot-page Postinitialize code that copies `pageSys.nFIsi` or `pageSys.nBIsi` into lot isi fields. That code would overwrite keyboard edits.
