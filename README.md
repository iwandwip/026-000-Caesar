# Caesar Control System

Caesar is an ESP32 control project for a two-side production process. It combines a Nextion HMI, a GM66 barcode scanner, and a DS3231 RTC. The firmware owns validation and process state; the HMI presents data and sends user actions.

## Repository Layout

```text
026-000-Caesar/
├── firmware/                         ESP32 firmware, local Arduino libraries, and setup notes
│   ├── CaesarFirmwareV1/             Main Arduino sketch
│   └── README.md                     Firmware architecture, wiring, and HMI contract
├── HMI/                              Versioned Nextion .HMI source files
├── nextion/                          HMI file used by extractor and extracted source views
│   ├── HMI CAESAR_12.HMI             Current extraction input
│   ├── Nextion2Text/                 Nextion2Text checkout
│   └── output/                       Extracted text and JSON per HMI revision
├── src/                              HMI extraction scripts
├── pcb/                              PCB design archive and extracted design files
├── assets/                           Manual, work instruction, and reference documents
├── docs/                             Design specifications and implementation plans
└── README.md
```

`HMI/` stores HMI revision history. Copy the revision you want to inspect into `nextion/` before running the extractor. The current extractor input is `nextion/HMI CAESAR_12.HMI`.

## System Overview

| Component | Responsibility |
| --- | --- |
| ESP32 | Validates operators, moulds, lots, downtime state, scanner input, and RTC data. |
| Nextion HMI | Displays process state, accepts manual input, and sends page and button events. |
| GM66 scanner | Sends barcode data to ESP32 through `Serial2`. |
| DS3231 RTC | Supplies timestamps for `pageSys.tNow` and downtime start records. |

The firmware stores shared process state in `pageSys` on the HMI. Firmware callbacks validate process actions.

See [`firmware/README.md`](firmware/README.md) for wiring, firmware modules, supported input flows, and HMI integration requirements.

## HMI Extraction

The extraction workflow converts a Nextion `.HMI` file into text and JSON for review, Git diffs, and firmware work. It only reads `.HMI` files. It cannot rebuild an `.HMI` file from extracted output.

### Requirements

- Linux shell or Bash
- Git
- Python 3.8 or newer
- A `.HMI` file directly inside `nextion/`

Check Python:

```bash
python3 --version
```

Create a virtual environment when needed:

```bash
python3 -m venv .venv
source .venv/bin/activate
```

The extraction scripts use Python standard-library modules only.

### Run Extraction

From repository root:

```bash
./src/nextion2text.sh
```

The script:

1. Finds `.HMI` and `.hmi` files directly under `nextion/`.
2. Clones [Nextion2Text](https://github.com/MMMZZZZ/Nextion2Text) into `nextion/Nextion2Text/` when absent.
3. Replaces previous output for each processed HMI file.
4. Writes text and JSON output to `nextion/output/<hmi-name>/`.

For `nextion/HMI CAESAR_12.HMI`, output appears under:

```text
nextion/output/HMI_CAESAR_12/
├── text/
└── json/
```

The script calls Nextion2Text with:

```bash
-e -s -j -p visual
```

| Flag | Purpose |
| --- | --- |
| `-e` | Includes empty events. |
| `-s` | Produces page, component, and event statistics. |
| `-j` | Produces JSON output. |
| `-p visual` | Includes visual properties such as coordinates, colors, fonts, and images. |

### Linux `ansi` Codec Error

Nextion2Text requests the Windows `ansi` codec. Linux Python does not provide that name. `src/run_nextion2text.py` registers `ansi` as `latin-1` before it starts Nextion2Text.

Run `./src/nextion2text.sh`; do not run `Nextion2Text.py` directly on Linux.

### Common Errors

`ERROR: no .HMI files found` means no `.HMI` file exists directly in `nextion/`. Copy the desired source file from `HMI/` first.

```bash
cp "HMI/HMI CAESAR_12.HMI" "nextion/HMI CAESAR_12.HMI"
./src/nextion2text.sh
```

If the script lacks execute permission:

```bash
chmod +x src/nextion2text.sh
```

## Project Documents

- `assets/MANUAL BOOK_CAESAR.docx`: Caesar manual.
- `assets/WORK INSTRUCTION_CAESAR.pdf`: work instruction.
- `assets/SKRIPSI_CAESAR.docx`: project thesis.
- `pcb/`: PCB source archive and extracted project files.
