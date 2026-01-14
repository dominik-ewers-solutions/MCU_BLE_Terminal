# McuBleTerminal ESP32 Wireless Serial over BLE

McuBleTerminal is a lightweight ESP32 library that provides a wireless serial terminal over Bluetooth Low Energy (BLE).
It is designed as a drop-in replacement for Serial, allowing you to use print, println and printf without a USB cable.
A companion mobile app acts as a BLE terminal, similar to a classic serial monitor.

---

## Features

- Wireless serial communication over BLE
- Drop-in replacement for Serial
- print, println and printf supported
- Bidirectional communication (TX and RX)
- No WiFi, no TCP/IP, no cloud
- Low power BLE based solution
- Compatible with Arduino IDE and PlatformIO
- Companion mobile app with terminal UI
- Demo mode in the app (no hardware required)

---

## Supported Hardware

- ESP32 using the Arduino framework

Other microcontrollers may be supported in the future.

---

## Installation

### Arduino IDE

1. Clone or download this repository
2. Copy it into your Arduino libraries folder
3. Restart the Arduino IDE

### PlatformIO

Add the library as a dependency:

ini:
lib_deps =
  https://github.com/dominik-ewers-solutions/MCU_BLE_Terminal


## Usage
#include <McuBleTerminal.h>

void setup() {
  WirelessSerial.begin("MyESP32");
  WirelessSerial.println("Booting...");
}

void loop() {
  WirelessSerial.printf("Millis: %lu\n", millis())
  while (WirelessSerial.available()) {
    char c = WirelessSerial.read();
    WirelessSerial.printf("RX: %c\n", c);
  }
  delay(1000);
}

The string passed to WirelessSerial.begin() is used as the BLE device name and will be shown in the mobile app.

## Mobile App

The companion app McuBleTerminal is currently NOT available for Android and iOS.

App features:

BLE device scan
Terminal style user interface
Monospace font and dark theme
Live log output
Text input for sending data to the MCU
Demo mode without BLE hardware
Multi-language user interface

## Security Notes

BLE communication is unencrypted by default
Intended for development and debugging purposes only
Not suitable for safety critical or production systems

## Design Philosophy

Keep the API simple
No custom protocol
No framing or message parsing
Raw byte stream similar to Serial
Minimal setup and minimal dependencies

## License
MIT License

## Contributions
Contributions, bug reports and feature requests are welcome.

## Disclaimer
This library is provided as is and intended as a development tool.
The authors take no responsibility for improper use.