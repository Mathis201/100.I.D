#include "ble_setup.h"
#include "game_info_service.h"
#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>

constexpr static int onboardDel = 2;

constexpr static int serialPrintDelay = 250;
static int lastTime = 0;

static bool deviceConnected = false;

// Setup callbacks onConnect and onDisconnect
class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        deviceConnected = true;
        digitalWrite(onboardDel, true);
        Serial.println("Device connected");
    };
    void onDisconnect(BLEServer *pServer) {
        deviceConnected = false;
        digitalWrite(onboardDel, false);
        pServer->getAdvertising()->stop();
        pServer->getAdvertising()->start();
        Serial.println("Device disconnected");
    }
};

static ServerCallbacks serverCallbacks{};
static BLEServer *pServer;

static BLECharacteristic *pPlayerInfo_GridCharacteristic;
static BLECharacteristic *pPlayerInfo_LastShotCoordsCharacteristic;
static BLECharacteristic *pPlayerInfo_LastShotResultCharacteristic;

static BLECharacteristic *pRobotInfo_LastShotCoordsCharacteristic;
static BLECharacteristic *pRobotInfo_LastShotResultCharacteristic;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial2.begin(9600);

    BLEDevice::init(ServerSettings::serverName);
    BLEServer *pServer = BLEDevice::createServer();

    setupGameInfoService(pServer);

    pServer->getAdvertising()->setMinInterval(ServerSettings::advertisingInterval);
    pServer->getAdvertising()->setMaxInterval(ServerSettings::advertisingInterval);
    pServer->getAdvertising()->start();
    pServer->setCallbacks(&serverCallbacks);

    pinMode(onboardDel, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    if ((millis() - lastTime) > serialPrintDelay) {
        lastTime = millis();
    }
}
