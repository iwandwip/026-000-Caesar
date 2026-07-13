# Arduino .ino Programming Rules

> Comprehensive guide to Arduino IDE preprocessing, file structure, and best practices

## Table of Contents
- [File Structure Requirements](#file-structure-requirements)
- [Preprocessing Pipeline](#preprocessing-pipeline)
- [Library Detection & Inclusion](#library-detection--inclusion)
- [Variable Scope & Access](#variable-scope--access)
- [Function Declaration Rules](#function-declaration-rules)
- [File Type Handling](#file-type-handling)
- [Common Pitfalls](#common-pitfalls)
- [Best Practices](#best-practices)
- [Examples](#examples)

---

## File Structure Requirements

### Rule 1: Folder Name Must Match Main File Name

**REQUIRED STRUCTURE:**
```
MyProject/              ← Folder name
├── MyProject.ino       ← Main file (MUST match folder name)
├── other_file.ino      ← Additional .ino files (optional)
├── MyClass.h           ← Header files (optional)
└── MyClass.cpp         ← C++ files (optional)
```

**❌ INVALID:**
```
MyProject/
└── sketch.ino          ❌ Name mismatch - Arduino IDE will refuse to open
```

### Rule 2: All .ino Files Must Be in Single Folder

**For .ino files:**
```
✅ VALID:
MyProject/
├── MyProject.ino
├── functions.ino
└── utils.ino

❌ INVALID:
MyProject/
├── MyProject.ino
└── src/
    └── functions.ino   ❌ .ino files cannot be in subfolders
```

**For .cpp/.h files:**
```
✅ VALID - Can use subfolders:
MyProject/
├── MyProject.ino
└── src/
    ├── MyClass.h       ✅ .h/.cpp can be in subfolders
    └── MyClass.cpp
```

### Rule 3: Main File is Entry Point

The file matching the folder name is **always processed first**, regardless of alphabetical order.

**Processing Order:**
```
MyProject/
├── MyProject.ino       ← 1. ALWAYS FIRST
├── aaa_config.ino      ← 2. Then alphabetically
├── bbb_sensors.ino     ← 3. Then alphabetically
└── zzz_utils.ino       ← 4. Last alphabetically
```

---

## Preprocessing Pipeline

### Stage 1: File Concatenation

All `.ino` files in the sketch folder are concatenated into a single `.cpp` file.

**Input:**
```
MyProject/
├── MyProject.ino
├── functions.ino
└── utils.ino
```

**Output (internal):**
```cpp
// Contents of MyProject.ino
// Contents of functions.ino
// Contents of utils.ino
```

### Stage 2: Add Arduino.h Include

If not already present, `#include <Arduino.h>` is automatically added at the top.

**Before:**
```cpp
void setup() {
  Serial.begin(9600);
}
```

**After preprocessing:**
```cpp
#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}
```

### Stage 3: Generate Function Prototypes

Arduino IDE scans for all function definitions and creates forward declarations.

**Before:**
```cpp
void setup() {
  myFunction();  // Can call before definition!
}

void myFunction() {
  Serial.println("Hello");
}
```

**After preprocessing:**
```cpp
#include <Arduino.h>

// Auto-generated prototypes
void setup();
void myFunction();

void setup() {
  myFunction();
}

void myFunction() {
  Serial.println("Hello");
}
```

### Stage 4: Add #line Directives

`#line` directives maintain accurate line numbers in error messages, so errors reference your original `.ino` file lines.

### Stage 5: Generate main() Function

Arduino automatically wraps your code with a `main()` function.

**Your code:**
```cpp
void setup() {
  // init code
}

void loop() {
  // main code
}
```

**Generated internally:**
```cpp
int main(void) {
  init();        // Arduino initialization
  setup();
  
  while(true) {
    loop();
  }
  
  return 0;
}
```

---

## Library Detection & Inclusion

### Rule 4: Library Includes Must Be in .ino Files

Arduino IDE **only scans .ino files** for library detection.

**✅ CORRECT - Include in .ino file:**
```cpp
// MyProject.ino
#include <Wire.h>
#include <SPI.h>
#include "MyClass.h"

void setup() {
  Wire.begin();
}
```

**❌ WRONG - Include only in .cpp:**
```cpp
// MyProject.ino
void setup() {
  Wire.begin();  // Wire library might not be linked!
}

// MyClass.cpp
#include <Wire.h>  // ⚠️ Arduino IDE won't detect this!
```

### Rule 5: Headers (.h) Can Include Libraries

**✅ CORRECT - Include library in header:**
```cpp
// MyClass.h
#ifndef MyClass_h
#define MyClass_h

#include <Wire.h>     // ✅ OK in header
#include <Arduino.h>

class MyClass {
  public:
    void begin();
};

#endif
```

```cpp
// MyProject.ino
#include "MyClass.h"  // Wire.h is included transitively

MyClass obj;

void setup() {
  obj.begin();  // ✅ Wire library is linked
}
```

**Why this works:** When Arduino preprocessor expands `#include "MyClass.h"`, it sees `#include <Wire.h>` and adds Wire library to the build.

### Rule 6: Include Chain Must Be Traceable from .ino

The chain of includes must start from a .ino file:

```
MyProject.ino → includes MyClass.h → includes Wire.h
                                   ↓
                            Wire library detected ✅
```

---

## Variable Scope & Access

### Rule 7: Global Variable Placement Strategy

**Best practice:** Declare all global variables in the **main file** (matches folder name).

**✅ RECOMMENDED:**
```cpp
// MyProject.ino (main file)
int globalCounter = 0;
String deviceName = "Arduino";
bool isConnected = false;

void setup() {
  // use variables
}
```

```cpp
// functions.ino (other file)
void incrementCounter() {
  globalCounter++;  // ✅ Can access - defined in main file (processed first)
}
```

**⚠️ PROBLEMATIC:**
```cpp
// zzz_variables.ino (processed last alphabetically)
int globalCounter = 0;

// aaa_functions.ino (processed first alphabetically)
void useCounter() {
  globalCounter++;  // ❌ ERROR - not yet declared!
}
```

### Rule 8: Cross-File Variable Access with extern

For variables defined in non-main .ino files, use `extern` declaration:

**✅ WITH EXTERN:**
```cpp
// MyProject.ino (main file)
extern int sensorValue;  // Declare it exists elsewhere

void setup() {
  Serial.println(sensorValue);  // ✅ OK
}
```

```cpp
// sensors.ino
int sensorValue = 0;  // Actual definition

void readSensor() {
  sensorValue = analogRead(A0);
}
```

---

## Function Declaration Rules

### Rule 9: Functions Work Across All .ino Files

**All functions from all .ino files are accessible everywhere** thanks to auto-generated prototypes.

```cpp
// MyProject.ino
void setup() {
  helperFromFile1();
  utilityFromFile2();
}

// file1.ino
void helperFromFile1() {
  Serial.println("From file 1");
}

// file2.ino  
void utilityFromFile2() {
  Serial.println("From file 2");
}
```

All of these work seamlessly - **order doesn't matter for functions**.

### Rule 10: Complex Signatures Need Manual Prototypes

Auto-prototype generation may fail for:

**Function pointers:**
```cpp
// Add manual prototype
void (*getHandler())(int);

void setup() {
  auto func = getHandler();
}

void (*getHandler())(int) {
  // implementation
}
```

**Default parameters with complex types:**
```cpp
// Add manual prototype
void processData(String data, int mode = 0);

void setup() {
  processData("test");
}

void processData(String data, int mode) {
  // implementation
}
```

---

## File Type Handling

### Rule 11: Different File Types, Different Rules

| File Type | Preprocessing | Concatenation | Location |
|-----------|--------------|---------------|----------|
| `.ino` | ✅ Full Arduino preprocessing | ✅ Concatenated together | Must be in root folder |
| `.pde` | ✅ Full Arduino preprocessing | ✅ Concatenated together | Must be in root folder |
| `.cpp` | ❌ Standard C++ compilation | ❌ Compiled separately | Can be in subfolders |
| `.c` | ❌ Standard C compilation | ❌ Compiled separately | Can be in subfolders |
| `.h` | ❌ No preprocessing (just copied) | ❌ Available for `#include` | Can be in subfolders |

### Rule 12: .cpp Files Follow Standard C++ Rules

When you create `.cpp` files in your Arduino project, **standard C++ rules apply**:

**MyClass.cpp:**
```cpp
#include "MyClass.h"  // ✅ Must include header
#include <Arduino.h>  // ✅ Must include Arduino.h

// ✅ Need manual function declarations
void helperFunction();  // Forward declaration

void MyClass::myMethod() {
  helperFunction();
}

void helperFunction() {
  Serial.println("Helper");
}
```

**No automatic features in .cpp:**
- ❌ No auto-include of Arduino.h
- ❌ No auto-function prototypes  
- ❌ No concatenation with other files
- ✅ Standard C++ compilation unit

---

## Common Pitfalls

### Pitfall 1: Assuming Alphabetical Variable Access

**❌ WRONG:**
```cpp
// zzz_config.ino
const int LED_PIN = 13;

// aaa_main.ino  
void setup() {
  pinMode(LED_PIN, OUTPUT);  // ❌ ERROR - LED_PIN not yet defined
}
```

**✅ FIX - Put variables in main file:**
```cpp
// MyProject.ino (main file)
const int LED_PIN = 13;

void setup() {
  pinMode(LED_PIN, OUTPUT);  // ✅ OK
}

// aaa_functions.ino
void blinkLED() {
  digitalWrite(LED_PIN, HIGH);  // ✅ OK - main file processed first
}
```

### Pitfall 2: Including .ino Files

**❌ NEVER DO THIS:**
```cpp
// MyProject.ino
#include "functions.ino"  // ❌ WRONG! Arduino already concatenates them
```

**✅ CORRECT:**
```cpp
// MyProject.ino
// Just write your code - functions.ino is auto-concatenated

void setup() {
  helperFunction();  // ✅ Can directly call from functions.ino
}
```

### Pitfall 3: Library Include Only in .cpp

**❌ PROBLEM:**
```cpp
// MySensor.cpp
#include <Wire.h>  // Arduino IDE might not detect Wire library

void initSensor() {
  Wire.begin();
}
```

**✅ SOLUTION:**
```cpp
// MySensor.h
#include <Wire.h>  // ✅ Include in header

class MySensor {
  public:
    void init();
};

// MyProject.ino
#include "MySensor.h"  // ✅ Now Wire is detected
```

---

## Best Practices

### 1. File Organization Strategy

**Recommended structure for medium-sized projects:**
```
MyProject/
├── MyProject.ino           # Main file: includes, globals, setup(), loop()
├── 01_config.ino           # Configuration and constants
├── 02_sensors.ino          # Sensor-related functions
├── 03_display.ino          # Display functions
├── 99_utilities.ino        # Utility functions
└── src/
    ├── MyClass.h           # Complex classes in .h/.cpp
    └── MyClass.cpp
```

**Benefits:**
- Numbered prefixes control concatenation order
- Clear separation of concerns
- Main file stays clean

### 2. Main File Template

```cpp
/*
 * MyProject - Description
 * Author: Your Name
 */

// ============================================
// LIBRARY INCLUDES
// ============================================
#include <Wire.h>
#include <SPI.h>
#include "MyClass.h"

// ============================================
// GLOBAL CONSTANTS
// ============================================
const int LED_PIN = 13;
const int SENSOR_PIN = A0;

// ============================================
// GLOBAL VARIABLES
// ============================================
int sensorValue = 0;
String deviceName = "Arduino";

// ============================================
// SETUP
// ============================================
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  initSensors();     // From sensors.ino
  initDisplay();     // From display.ino
}

// ============================================
// LOOP
// ============================================
void loop() {
  readSensors();
  updateDisplay();
  delay(100);
}
```

### 3. When to Use .ino vs .cpp/.h

**Use .ino files for:**
- ✅ Simple helper functions
- ✅ Arduino-specific code
- ✅ Quick prototyping

**Use .cpp/.h files for:**
- ✅ Complex classes
- ✅ Reusable libraries
- ✅ Proper encapsulation
- ✅ Code shared across projects

### 4. Testing Strategy

**Use conditional compilation for debugging:**
```cpp
// MyProject.ino
#define DEBUG_MODE  // Comment out for production

#ifdef DEBUG_MODE
  #define DEBUG_PRINT(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
#endif

void loop() {
  int value = readSensor();
  DEBUG_PRINT("Sensor: " + String(value));  // Only prints in debug mode
}
```

---

## Examples

### Example 1: Simple Multi-File Project

**LEDController.ino:**
```cpp
const int LED_PIN = 9;
int brightness = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  blinkLED();      // From blink.ino
  delay(1000);
  fadeLED();       // From fade.ino
  delay(1000);
}
```

**blink.ino:**
```cpp
void blinkLED() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
```

**fade.ino:**
```cpp
void fadeLED() {
  for(int i = 0; i <= 255; i++) {
    analogWrite(LED_PIN, i);
    delay(10);
  }
}
```

### Example 2: Project with Classes

**SensorLogger.ino:**
```cpp
#include <SD.h>
#include "src/Logger.h"

const int SENSOR_PIN = A0;
Logger dataLogger;

void setup() {
  Serial.begin(9600);
  dataLogger.begin();
  initSensor();
}

void loop() {
  int value = readSensor();
  dataLogger.log(value);
  delay(1000);
}
```

**functions.ino:**
```cpp
void initSensor() {
  pinMode(SENSOR_PIN, INPUT);
}

int readSensor() {
  return analogRead(SENSOR_PIN);
}
```

**src/Logger.h:**
```cpp
#ifndef Logger_h
#define Logger_h

#include <Arduino.h>
#include <SD.h>

class Logger {
  private:
    File logFile;
  public:
    void begin();
    void log(int value);
};

#endif
```

**src/Logger.cpp:**
```cpp
#include "Logger.h"

void Logger::begin() {
  SD.begin(10);
  logFile = SD.open("data.txt", FILE_WRITE);
}

void Logger::log(int value) {
  if(logFile) {
    logFile.println(value);
    logFile.flush();
  }
}
```

---

## Summary Checklist

### File Structure ✓
- [ ] Main file name matches folder name
- [ ] All .ino files in root folder (no subfolders)
- [ ] .cpp/.h files can be in subfolders

### Preprocessing Understanding ✓
- [ ] Know that .ino files are concatenated
- [ ] Understand concatenation order (main first, then alphabetically)
- [ ] Know function prototypes are auto-generated

### Library Management ✓
- [ ] All library #includes in .ino files (preferably main file)
- [ ] Or #includes in .h files that are included from .ino

### Variable Management ✓
- [ ] Global variables in main file
- [ ] Use extern for cross-file variable access if needed

### Function Organization ✓
- [ ] Functions can be called from any .ino file
- [ ] Manual prototypes for complex signatures

---

**Version:** 2.0 (500-line edition)  
**Last Updated:** 2024  
**Standard:** Cursor IDE compatible
