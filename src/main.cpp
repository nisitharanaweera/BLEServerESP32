#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

const String DeviceDisplayName = "ESPServer2";
BLEUUID serviceUUID("43721262-a0cd-433c-a1bc-36caba2b4c59"); // service UUID
BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");    // characteristic UUID



BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
int value = 0;
String msg = "";
String Sval = "";


void BLEServerInit();

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        Serial.println("Connected to client");
    }

    void onDisconnect(BLEServer *pServer)
    {
        Serial.println("Disconnected from client");
        //Restart Advertising the service
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        pAdvertising->start();
    }
};


void setup()
{
    Serial.begin(115200);
    Serial.println("Starting BLE Server...");

    BLEDevice::init("ESPServer2");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(serviceUUID);

    pCharacteristic = pService->createCharacteristic(
        charUUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);

    msg = "msg from S2: init";
    const char *charArray = msg.c_str();
    uint8_t *byteArray = (uint8_t *)charArray;
    size_t BAlength = strlen((const char *)byteArray);

    pCharacteristic->setValue(byteArray, BAlength);

    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

void loop()
{
    value++;
    Sval = std::to_string(value).c_str();
    msg = "msg from S2:" + Sval;
    const char *charArray = msg.c_str();
    uint8_t *byteArray = (uint8_t *)charArray;
    size_t BAlength = strlen((const char *)byteArray);

    pCharacteristic->setValue(byteArray, BAlength);

    pCharacteristic->notify();
    Serial.print("Data Updated :");
    Serial.println(Sval);

    delay(2000);
}
