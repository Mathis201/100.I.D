#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#define SERVER_NAME "100ID_ESP32"
#define SERVICE_UUID "37d5098d-a1bd-471e-8794-aae7bf8ed532"
#define ADVERTISING_INTERVAL 100 // ms

#define LED_STATE_CHARACTERISTIC_UUID "4ca76883-83e7-4c4e-a3da-b433e5e8fd2a"

#define ONBOARD_DEL 2

constexpr int serialPrintDelay = 250;
int lastTime = 0;

bool deviceConnected = false;
int ledOn = 0;

// Setup callbacks onConnect and onDisconnect
class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        deviceConnected = true;
        Serial.println("Device connected");
    };
    void onDisconnect(BLEServer *pServer) {
        deviceConnected = false;
        Serial.println("Device disconnected");
    }
};

static ServerCallbacks serverCallbacks{};
static BLEDescriptor ledStateDescriptor{BLEUUID((uint16_t)0x2903)};
static BLECharacteristic *pLedStateCharacteristic;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial2.begin(9600);

    BLEDevice::init(SERVER_NAME);
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(&serverCallbacks);

    BLEService *pService = pServer->createService(SERVICE_UUID);
    pLedStateCharacteristic = pService->createCharacteristic(LED_STATE_CHARACTERISTIC_UUID,
                                                             BLECharacteristic::PROPERTY_READ |
                                                                 BLECharacteristic::PROPERTY_WRITE |
                                                                 BLECharacteristic::PROPERTY_INDICATE);

    ledStateDescriptor.setValue("LED State");
    pLedStateCharacteristic->addDescriptor(&ledStateDescriptor);
    pLedStateCharacteristic->addDescriptor(new BLE2902());
    pLedStateCharacteristic->setValue(ledOn);

    pService->start();

    pServer->getAdvertising()->addServiceUUID(SERVICE_UUID);
    pServer->getAdvertising()->setMinInterval(ADVERTISING_INTERVAL);
    pServer->getAdvertising()->setMaxInterval(ADVERTISING_INTERVAL);
    pServer->getAdvertising()->start();

    pinMode(ONBOARD_DEL, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    if ((millis() - lastTime) > serialPrintDelay) {
        int newVal = *pLedStateCharacteristic->getData();
        Serial.println(newVal);
        Serial2.println(newVal);
        digitalWrite(ONBOARD_DEL, newVal);
        lastTime = millis();
    }
}
