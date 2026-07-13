# Firmware Notes

## Nextion Page ID Mapping

Page ID follows page order in Nextion Editor. Use these IDs when declaring `NexPage`, `NexButton`, `NexText`, `NexNumber`, and other Nextion components in Arduino code.

| Page ID | Page Name |
| --- | --- |
| 0 | pageDashboard |
| 1 | pageLoginF |
| 2 | pageLoginB |
| 3 | pageMldF |
| 4 | pageMldB |
| 5 | pageLotF |
| 6 | pageLotB |
| 7 | pageOutputNG |
| 8 | pageInputNGF |
| 9 | pageInputNGB |
| 10 | pageDtF |
| 11 | pageDtB |
| 12 | pageDtInfoF |
| 13 | pageDtInfoB |
| 14 | pageDtMc |
| 15 | pageDtMcInfo |
| 16 | pageSim |
| 17 | pageSys |
| 18 | keybdA |
| 19 | keybdB |

## Login Page IDs

`pageLoginF` uses Page ID `1`.

`pageLoginB` uses Page ID `2`.

## Serial Selection

Nextion serial is selected in `firmware/libraries/ITEADLIB_Arduino_Nextion/NexConfig.h`:

```cpp
#define nexSerial Serial
```

Use `Serial`, `Serial1`, or `Serial2` based on wiring. Current setup uses `Serial` for both Nextion Editor simulator and real Nextion display.

GM66 scanner serial is selected in `firmware/CaesarFirmwareV1/config.h`:

```cpp
#define SCANNER_SERIAL Serial2
#define SCANNER_RX_PIN 16
#define SCANNER_TX_PIN 17
#define SCANNER_BAUD 9600
```

Use `Serial`, `Serial1`, or `Serial2` for `SCANNER_SERIAL`. Current setup uses `Serial2` with ESP32 default RX2/TX2 pins.

Do not open Arduino Serial Monitor on the ESP32 USB COM port while Nextion Editor Debug uses `Serial`.

ITEADLIB debug serial is disabled in `NexConfig.h` because debug text would share the same serial channel as Nextion protocol when `nexSerial` is `Serial`.

## GM66 Scanner

GM66 uses ESP32 `Serial2` at `9600` baud, `SERIAL_8N1`.

| GM66 Wire | ESP32 Pin |
| --- | --- |
| TX | GPIO16 / Serial2 RX |
| RX | GPIO17 / Serial2 TX |
| VCC | 5V |
| GND | GND |

Open `pageLoginF` or `pageLoginB`, then scan operator ID. Firmware uses current login page to choose front or back login, validates ID from `OPERATORS[]`, updates `pageSys`, and writes operator name to `tNameF` or `tNameB`.

Scanner stays on physical ESP32 `Serial2`. Nextion stays on ESP32 `Serial` for both simulator and real mode in current setup.

For page tracking, set each Nextion page Preinitialize event to its page ID:

```text
pageDashboard: printh 65 00 00 00 FF FF FF
pageLoginF:   printh 65 01 00 00 FF FF FF
pageLoginB:   printh 65 02 00 00 FF FF FF
...
pageSys:      printh 65 17 00 00 FF FF FF
keybdA:       printh 65 18 00 00 FF FF FF
keybdB:       printh 65 19 00 00 FF FF FF
```

The fourth byte is `00`, so firmware tracks page changes with `attachPop()` on page objects.
