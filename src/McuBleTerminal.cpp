#include "McuBleTerminal.h"

#if defined(ARDUINO_ARCH_ESP32)
  #include <BLEDevice.h>
  #include <BLEServer.h>
  #include <BLEUtils.h>
  #include <BLE2902.h>
#elif defined(ARDUINO_ARCH_NRF52)
  #include <ArduinoBLE.h>
#else
  #warning "WirelessSerial: Unsupported architecture (no ESP32 / nRF52)."
#endif

static const size_t RX_BUFFER_SIZE = 256;
static uint8_t rxBuffer[RX_BUFFER_SIZE];
static volatile size_t rxHead = 0;
static volatile size_t rxTail = 0;

#if defined(ARDUINO_ARCH_ESP32)
static BLECharacteristic* txChar = nullptr;
static bool clientConnected = false;

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer*) override {
    clientConnected = true;
  }

  void onDisconnect(BLEServer*) override {
    clientConnected = false;
    BLEDevice::startAdvertising();
  }
};

class RxCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* characteristic) override {
    auto value = characteristic->getValue();
    size_t len = value.length();

    for (size_t i = 0; i < len; i++) {
      uint8_t c = (uint8_t)value[i];
      size_t next = (rxHead + 1) % RX_BUFFER_SIZE;
      if (next != rxTail) {
        rxBuffer[rxHead] = c;
        rxHead = next;
      }
    }
  }
};

#endif

#if defined(ARDUINO_ARCH_NRF52)

static BLEService wsService(WS_SERVICE_UUID);

static BLECharacteristic txChar(
  WS_CHAR_TX_UUID,
  BLENotify,
  20
);

static BLECharacteristic rxChar(
  WS_CHAR_RX_UUID,
  BLEWrite | BLEWriteWithoutResponse,
  20
);

static void onRxWritten(BLEDevice, BLECharacteristic characteristic) {
  const uint8_t* data = characteristic.value();
  int len = characteristic.valueLength();

  for (int i = 0; i < len; i++) {
    uint8_t c = data[i];
    size_t next = (rxHead + 1) % RX_BUFFER_SIZE;
    if (next != rxTail) {
      rxBuffer[rxHead] = c;
      rxHead = next;
    }
  }
}

#endif 

WirelessSerialClass WirelessSerial;

bool WirelessSerialClass::begin(const char* deviceName) {
  #if defined(ARDUINO_ARCH_ESP32)

  BLEDevice::init(deviceName);

  BLEServer* server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  BLEService* service = server->createService(WS_SERVICE_UUID);

  txChar = service->createCharacteristic(
    WS_CHAR_TX_UUID,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  txChar->addDescriptor(new BLE2902());

  BLECharacteristic* rxChar = service->createCharacteristic(
  WS_CHAR_RX_UUID,
  BLECharacteristic::PROPERTY_WRITE |
  BLECharacteristic::PROPERTY_WRITE_NR
);

  rxChar->setCallbacks(new RxCallbacks());

  service->start();

  BLEAdvertising* advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(WS_SERVICE_UUID);
  advertising->setScanResponse(true);
  advertising->setMinPreferred(0x06);
  advertising->setMinPreferred(0x12);

  BLEDevice::startAdvertising();

  return true;

#elif defined(ARDUINO_ARCH_NRF52)
if (!BLE.begin()) {
    return false;
  }

  BLE.setLocalName(deviceName);
  BLE.setAdvertisedService(wsService);

  wsService.addCharacteristic(txChar);
  wsService.addCharacteristic(rxChar);
  BLE.addService(wsService);

  rxChar.setEventHandler(BLEWritten, onRxWritten);

  BLE.advertise();
  return true;

#else

  (void)deviceName;
  return false;

#endif
}

void WirelessSerialClass::end() {
  #if defined(ARDUINO_ARCH_ESP32)
  BLEDevice::deinit();
  #elif defined(ARDUINO_ARCH_NRF52)
  BLE.end();
  #endif
}

void WirelessSerialClass::loop() {
#if defined(ARDUINO_ARCH_NRF52)
  BLE.poll();
#endif
}

bool WirelessSerialClass::connected() {
  #if defined(ARDUINO_ARCH_ESP32)
    return clientConnected;
  #elif defined(ARDUINO_ARCH_NRF52)
    return BLE.connected();
  #else
    return false;
  #endif
}

size_t WirelessSerialClass::write(uint8_t c) {
  return write(&c, 1);
}

size_t WirelessSerialClass::write(const uint8_t* buffer, size_t size) {
  #if defined(ARDUINO_ARCH_ESP32)
  if (!clientConnected || !txChar) return size;

  const size_t mtu = 20;
  size_t offset = 0;

  while (offset < size) {
    size_t chunk = min(mtu, size - offset);
    txChar->setValue((uint8_t*)(buffer + offset), chunk);
    txChar->notify();
    offset += chunk;
    delay(1);
  }
  return size;
  #elif defined(ARDUINO_ARCH_NRF52)
  if (!BLE.connected()) return size;

  const size_t mtu = 20;
  size_t offset = 0;

  while (offset < size) {
    size_t chunk = min(mtu, size - offset);
    txChar.writeValue(buffer + offset, chunk);
    offset += chunk;
    delay(1);
  }
  return size;

#else

  return size;

#endif
}

int WirelessSerialClass::available() {
  return (RX_BUFFER_SIZE + rxHead - rxTail) % RX_BUFFER_SIZE;
}

int WirelessSerialClass::read() {
  if (rxHead == rxTail) return -1;
  uint8_t c = rxBuffer[rxTail];
  rxTail = (rxTail + 1) % RX_BUFFER_SIZE;
  return c;
}

int WirelessSerialClass::peek() {
  if (rxHead == rxTail) return -1;
  return rxBuffer[rxTail];
}

void WirelessSerialClass::flush() {
}

static String rxLine;

bool WirelessSerialClass::availableLine() {
  while (available()) {
    char c = read();

    if (c == '\r') {
      continue;
    }

    if (c == '\n') {
      return true;
    }

    rxLine += c;

    if (rxLine.length() > 128) {
      rxLine = "";
    }
  }
  return false;
}

String WirelessSerialClass::readLine() {
  String line = rxLine;
  rxLine = "";
  return line;
}
