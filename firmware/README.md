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

Select mode in `firmware/CaesarFirmwareV1/config.h`.

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
