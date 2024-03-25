#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
uint8_t value = 0;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        Serial.println("Connected to client");
    }

    void onDisconnect(BLEServer* pServer) {
        Serial.println("Disconnected from client");
    }
};

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE Server...");

    BLEDevice::init("ESPServer2");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(BLEUUID("43721262-a0cd-433c-a1bc-36caba2b4c59")); // UUID for the service

    pCharacteristic = pService->createCharacteristic(
                       BLEUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"), // UUID for the characteristic
                       BLECharacteristic::PROPERTY_READ |
                       BLECharacteristic::PROPERTY_WRITE |
                       BLECharacteristic::PROPERTY_NOTIFY
                     );

    pCharacteristic->setValue(&value, 1);

    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

void loop() {
    delay(5000);
    value++; // Incrementing value every 5 seconds
    pCharacteristic->setValue(&value, 1);
    pCharacteristic->notify();
}
