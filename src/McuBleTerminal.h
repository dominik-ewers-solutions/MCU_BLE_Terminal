#pragma once

#include <Arduino.h>
#include <Stream.h>

#define WS_SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define WS_CHAR_TX_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define WS_CHAR_RX_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class WirelessSerialClass : public Stream {
public:
  bool begin(const char* deviceName);
  void end();

  void loop();

  bool connected();

  // TX (ESP -> App)
  size_t write(uint8_t c) override;
  size_t write(const uint8_t* buffer, size_t size) override;

  // RX (App -> ESP)
  int available() override;
  int read() override;
  int peek() override;
  void flush() override;
  bool availableLine();
  String readLine();
};

extern WirelessSerialClass WirelessSerial;
