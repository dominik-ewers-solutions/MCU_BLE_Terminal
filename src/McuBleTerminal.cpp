#include "McuBleTerminal.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

static BLECharacteristic* txChar = nullptr;
static bool clientConnected = false;

static const size_t RX_BUFFER_SIZE = 256;
static uint8_t rxBuffer[RX_BUFFER_SIZE];
static volatile size_t rxHead = 0;
static volatile size_t rxTail = 0;

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
    String value = characteristic->getValue();

    for (size_t i = 0; i < value.length(); i++) {
      uint8_t c = value[i];
      size_t next = (rxHead + 1) % RX_BUFFER_SIZE;
      if (next != rxTail) {
        rxBuffer[rxHead] = c;
        rxHead = next;
      }
    }
  }
};

WirelessSerialClass WirelessSerial;

bool WirelessSerialClass::begin(const char* deviceName) {
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
    BLECharacteristic::PROPERTY_WRITE
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
}

void WirelessSerialClass::end() {
  BLEDevice::deinit();
}

bool WirelessSerialClass::connected() {
  return clientConnected;
}

size_t WirelessSerialClass::write(uint8_t c) {
  return write(&c, 1);
}

size_t WirelessSerialClass::write(const uint8_t* buffer, size_t size) {
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
