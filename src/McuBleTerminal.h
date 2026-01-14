#pragma once

#include <Arduino.h>
#include <Stream.h>

class WirelessSerialClass : public Stream {
public:
  bool begin(const char* deviceName);
  void end();

  bool connected();

  // TX (ESP -> App)
  size_t write(uint8_t c) override;
  size_t write(const uint8_t* buffer, size_t size) override;

  // RX (App -> ESP)
  int available() override;
  int read() override;
  int peek() override;
  void flush() override;

private:
  void notify(const uint8_t* data, size_t len);
};

extern WirelessSerialClass WirelessSerial;
