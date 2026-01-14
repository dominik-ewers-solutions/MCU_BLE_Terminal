#include <Arduino.h>
#include <McuBleTerminal.h>

const int SENSOR_PIN = 34;

void setup() {
  WirelessSerial.begin("MyESP32");
  WirelessSerial.println("Sensor values example");
}

void loop() {
  int value = analogRead(SENSOR_PIN);
  WirelessSerial.print("Sensor value: ");
  WirelessSerial.println(value);
  delay(1000);
}
