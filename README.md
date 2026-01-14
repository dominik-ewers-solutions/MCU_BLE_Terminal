McuBleTerminal ESP32 Wireless Serial over BLE

McuBleTerminal is a lightweight ESP32 library that provides a wireless serial terminal over Bluetooth Low Energy (BLE).

It is designed as a drop-in replacement for the Arduino Serial interface and allows you to use print, println and printf without a USB cable.
A companion mobile app acts as a BLE serial monitor and terminal.

The goal of this library is simple:
Use your phone as a wireless Serial Monitor for debugging, logging and simple interaction with your ESP32.


FEATURES

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


SUPPORTED HARDWARE

- ESP32 using the Arduino framework

Other microcontrollers may be supported in the future.


INSTALLATION

Arduino IDE

1. Clone or download this repository
2. Copy it into your Arduino libraries folder
3. Restart the Arduino IDE

PlatformIO

Add the library as a dependency:

lib_deps =
  https://github.com/dominik-ewers-solutions/MCU_BLE_Terminal


BASIC USAGE

Include the library:

#include <McuBleTerminal.h>

Initialize the wireless serial interface in setup():

void setup() {
  WirelessSerial.begin("MyESP32");
  WirelessSerial.println("Booting...");
}

The string passed to WirelessSerial.begin() is used as the BLE device name.
This name will be shown in the mobile app during scanning.


SENDING DATA (ESP32 -> APP)

You can use the same API you know from Serial:

WirelessSerial.print("Value: ");
WirelessSerial.println(123);
WirelessSerial.printf("Millis: %lu\n", millis());

Example:

void loop() {
  WirelessSerial.printf("Millis: %lu\n", millis());
  delay(1000);
}

This is useful for:
- Debug output
- Sensor values
- State changes
- Log messages


RECEIVING DATA (APP -> ESP32)

The app can send text to the ESP32.
Received data can be handled byte-wise or line-based.


BYTE-WISE INPUT (LOW LEVEL)

This behaves similar to Serial.read():

void loop() {
  while (WirelessSerial.available()) {
    char c = WirelessSerial.read();
    WirelessSerial.printf("RX: %c\n", c);
  }
}

This is useful if you want to process individual characters.


LINE-BASED INPUT (RECOMMENDED)

For most use cases, line-based input is much easier.
The library provides availableLine() and readLine().

Example:

void loop() {
  if (WirelessSerial.availableLine()) {
    String line = WirelessSerial.readLine();
    WirelessSerial.print("Received: ");
    WirelessSerial.println(line);
  }
}

A line is completed when a newline character is received.
Carriage return characters are ignored.


COMMAND STYLE USAGE

A common pattern is to use text commands:

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

This allows simple control interfaces without any custom protocol.


ECHO TEST (DEBUGGING)

To verify that RX and TX are working correctly, you can use an echo test:

void loop() {
  if (WirelessSerial.availableLine()) {
    String line = WirelessSerial.readLine();
    WirelessSerial.print("ECHO: ");
    WirelessSerial.println(line);
  }
}

If you type text in the app and receive it back unchanged, the connection is working.


WHY THIS IS USEFUL

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


MOBILE APP

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


SECURITY NOTES

- BLE communication is unencrypted by default
- Intended for development and debugging purposes
- Not suitable for safety critical or production systems
- Do not expose sensitive data


DESIGN PHILOSOPHY

- Keep the API simple
- No custom protocol
- No framing or parsing enforced
- Raw stream similar to Serial
- Minimal setup
- Minimal dependencies
- Predictable behavior


LICENSE

MIT License


CONTRIBUTIONS

Contributions, bug reports and feature requests are welcome.


DISCLAIMER

This library is provided as is and intended as a development tool.
The authors take no responsibility for improper use.
