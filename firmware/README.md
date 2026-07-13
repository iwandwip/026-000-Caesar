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

## Nextion Serial Mode

Select sketch mode in `firmware/CaesarFirmwareV1/config.h`.

Windows simulator mode:

```cpp
#define NEXTION_MODE_SIMULATOR 1
#define NEXTION_MODE_DEVICE 0
```

Hardware device mode:

```cpp
#define NEXTION_MODE_SIMULATOR 0
#define NEXTION_MODE_DEVICE 1
```

| Mode | `nexSerial` | Use Case |
| --- | --- | --- |
| Simulator | `Serial` | Nextion Editor Debug -> User MCU Input via ESP32 USB COM port |
| Device | `Serial2` | Real Nextion display wired to ESP32 RX2/TX2 |

Simulator mode uses ESP32 USB serial. Do not open Arduino Serial Monitor on the same COM port while Nextion Editor Debug uses it.

Device mode uses ESP32 `Serial2.begin(115200)` with default ESP32 pins: RX2 GPIO16 and TX2 GPIO17.

## ITEADLIB Nextion Serial Mode

The library under `firmware/libraries/ITEADLIB_Arduino_Nextion/` is modified for V1 simulator support. Library `.cpp` files do not read `firmware/CaesarFirmwareV1/config.h`, so `NexConfig.h` has its own mode selector.

Simulator mode in `firmware/libraries/ITEADLIB_Arduino_Nextion/NexConfig.h`:

```cpp
#define NEXTION_LIBRARY_MODE_SIMULATOR 1
#define NEXTION_LIBRARY_MODE_DEVICE 0
```

Hardware device mode in `firmware/libraries/ITEADLIB_Arduino_Nextion/NexConfig.h`:

```cpp
#define NEXTION_LIBRARY_MODE_SIMULATOR 0
#define NEXTION_LIBRARY_MODE_DEVICE 1
```

Keep `config.h` and `NexConfig.h` modes matched.

Simulator mode disables ITEADLIB debug serial because debug text and Nextion protocol share the same ESP32 USB serial port.

## GM67 Scanner

GM67 uses ESP32 `Serial1` at `9600` baud, `SERIAL_8N1`.

| GM67 Wire | ESP32 Pin |
| --- | --- |
| TX | GPIO2 / Serial1 RX |
| RX | GPIO4 / Serial1 TX |
| VCC | 5V |
| GND | GND |

Open `pageLoginF` or `pageLoginB`, then scan operator ID. Firmware uses current login page to choose front or back login, validates ID from `OPERATORS[]`, updates `pageSys`, and writes operator name to `tNameF` or `tNameB`.

Scanner stays on physical ESP32 `Serial1` in both simulator mode and device mode. Nextion simulator only changes display serial to ESP32 USB `Serial`.

For page tracking, set these Nextion Preinitialize events:

```text
pageLoginF: printh 65 01 00 00 FF FF FF
pageLoginB: printh 65 02 00 00 FF FF FF
```

Keep `bBackF` and `bBackB` Send Component ID on release so firmware clears current login-page state when leaving login page.
