# McuBleTerminal
## ESP32 Wireless Serial over BLE

McuBleTerminal is a lightweight ESP32 library that provides a wireless serial terminal over Bluetooth Low Energy (BLE).

It is designed as a drop-in replacement for the Arduino `Serial` interface and allows you to use `print`, `println` and `printf` without a USB cable.
A companion mobile app acts as a BLE serial monitor and terminal.

The goal of this library is simple:
Use your phone as a wireless Serial Monitor for debugging, logging and simple interaction with your ESP32.

---

## Features

- Wireless serial communication over BLE
- Drop-in replacement for Serial
- print, println and printf supported
- Bidirectional communication (TX and RX)
- Line-based input support
- No WiFi, no TCP/IP, no cloud
- Low power BLE based solution
- Arduino IDE and PlatformIO compatible
- Companion mobile app with terminal UI
- Demo mode in the app (no hardware required)

---

## Supported Hardware

- ESP32 using the Arduino framework

---

## Installation

### Arduino IDE

1. Clone or download this repository
2. Copy it into your Arduino libraries folder
3. Restart the Arduino IDE

### PlatformIO

```
lib_deps =
  https://github.com/dominik-ewers-solutions/MCU_BLE_Terminal
```

---

## Basic Usage

```
#include <McuBleTerminal.h>

void setup() {
  WirelessSerial.begin("MyESP32");
  WirelessSerial.println("Booting...");
}

void loop() {
  WirelessSerial.printf("Millis: %lu\n", millis());
  delay(1000);
}
```

---

## Line Based Input Example

```
void loop() {
  if (WirelessSerial.availableLine()) {
    String line = WirelessSerial.readLine();
    WirelessSerial.print("ECHO: ");
    WirelessSerial.println(line);
  }
}
```

---

## Why this is useful

- Wireless debugging without USB
- Enclosures without physical access
- Quick configuration and testing
- Simple command interfaces

---

## License

MIT License
