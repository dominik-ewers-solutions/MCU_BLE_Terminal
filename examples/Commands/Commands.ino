#include <Arduino.h>
#include <McuBleTerminal.h>

const int LED_PIN = LED_BUILTIN;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  WirelessSerial.begin("MyESP32");
  WirelessSerial.println("Command example ready");
  WirelessSerial.println("Commands: ledon, ledoff");
}

void loop() {
  if (WirelessSerial.availableLine()) {
    String cmd = WirelessSerial.readLine();

    if (cmd == "ledon") {
      digitalWrite(LED_PIN, HIGH);
      WirelessSerial.println("LED ON");
    }
    else if (cmd == "ledoff") {
      digitalWrite(LED_PIN, LOW);
      WirelessSerial.println("LED OFF");
    }
    else {
      WirelessSerial.print("Unknown command: ");
      WirelessSerial.println(cmd);
    }
  }
}
