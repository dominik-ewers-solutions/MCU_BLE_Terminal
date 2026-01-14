#include <McuBleTerminal.h>

void setup() {
  WirelessSerial.begin("MyESP32");
  WirelessSerial.println("BLE Echo example ready");
}

void loop() {
  if (WirelessSerial.availableLine()) {
    String line = WirelessSerial.readLine();
    WirelessSerial.print("ECHO: ");
    WirelessSerial.println(line);
  }
}
