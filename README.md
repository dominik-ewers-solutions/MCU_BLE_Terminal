# McuBleTerminal
## ESP32 Wireless Serial over BLE

McuBleTerminal is a lightweight ESP32 library that provides a wireless serial terminal over Bluetooth Low Energy (BLE).

It is designed as a drop-in replacement for the Arduino Serial interface and allows you to use print, println and printf without a USB cable.
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

Other microcontrollers may be supported in the future.

---

## Installation

### Arduino IDE

1. Clone or download this repository
2. Copy it into your Arduino libraries folder
3. Restart the Arduino IDE

### PlatformIO

Add the library as a dependency:

```ini
lib_deps =
  https://github.com/dominik-ewers-solutions/MCU_BLE_Terminal
```

---

## Basic Usage

```cpp
#include <McuBleTerminal.h>

void setup() {
  WirelessSerial.begin("MyESP32");
  WirelessSerial.println("Booting...");
}
```

The string passed to WirelessSerial.begin() is used as the BLE device name.
This name will be shown in the mobile app during scanning.

---

## Sending Data (ESP32 -> App)

```cpp
WirelessSerial.print("Value: ");
WirelessSerial.println(123);
WirelessSerial.printf("Millis: %lu\n", millis());
```

Example:

```cpp
void loop() {
  WirelessSerial.printf("Millis: %lu\n", millis());
  delay(1000);
}
```

This is useful for:
- Debug output
- Sensor values
- State changes
- Log messages

---

## Receiving Data (App -> ESP32)

### Byte-wise Input (Low Level)

```cpp
void loop() {
  while (WirelessSerial.available()) {
    char c = WirelessSerial.read();
    WirelessSerial.printf("RX: %c\n", c);
  }
}
```

### Line-based Input (Recommended)

```cpp
void loop() {
  if (WirelessSerial.availableLine()) {
    String line = WirelessSerial.readLine();
    WirelessSerial.print("Received: ");
    WirelessSerial.println(line);
  }
}
```

A line is completed when a newline character is received.
Carriage return characters are ignored.

---

## Command Style Usage

```cpp
void loop() {
  if (WirelessSerial.availableLine()) {
    String cmd = WirelessSerial.readLine();

    if (cmd == "ledon") {
      digitalWrite(LED_BUILTIN, HIGH);
      WirelessSerial.println("LED ON");
    }
    else if (cmd == "ledoff") {
      digitalWrite(LED_BUILTIN, LOW);
      WirelessSerial.println("LED OFF");
    }
    else {
      WirelessSerial.print("Unknown command: ");
      WirelessSerial.println(cmd);
    }
  }
}
```

---

## Echo Test (Debugging)

```cpp
void loop() {
  if (WirelessSerial.availableLine()) {
    String line = WirelessSerial.readLine();
    WirelessSerial.print("ECHO: ");
    WirelessSerial.println(line);
  }
}
```

If you type text in the app and receive it back unchanged, the connection is working.

---

## Why this is useful

McuBleTerminal is useful when:
- You want to debug without a USB cable
- The ESP32 is installed in a closed enclosure
- You want to log values on a phone
- You need a simple command interface
- USB access is inconvenient or impossible

Typical use cases:
- Sensor debugging
- Configuration during development
- Field testing
- Simple device control
- Wireless logging

---

## Mobile App

The companion app McuBleTerminal is intended for Android and iOS.

App features:
- BLE device scan
- Terminal style user interface
- Monospace font
- Dark and light mode
- Live log output
- Text input for sending data
- Demo mode without BLE hardware
- Multi-language user interface

The app is designed to behave similar to a classic serial monitor.
Currently the App is not available Apple's App Store, but it will be Soon.

Google Play: https://play.google.com/store/apps/details?id=com.desolutions.mcu_ble_terminal

---

## Security Notes

- BLE communication is unencrypted by default
- Intended for development and debugging purposes
- Not suitable for safety critical or production systems
- Do not expose sensitive data

---

## Design Philosophy

- Keep the API simple
- No custom protocol
- No framing or parsing enforced
- Raw stream similar to Serial
- Minimal setup
- Minimal dependencies
- Predictable behavior

---

## License

MIT License

---

## Contributions

Contributions, bug reports and feature requests are welcome.

---

## Disclaimer

This library is provided as is and intended as a development tool.
The authors take no responsibility for improper use.
