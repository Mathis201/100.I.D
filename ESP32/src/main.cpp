#include "BluetoothCommunication/BLECharacteristicManager.h"
#include "ble_setup.h"
#include "data_management/serial_manager.h"
#include "services/game_info_service.h"
#include "services/player_info_service.h"
#include "services/robot_info_service.h"

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>

constexpr static int onboardDel = 2;

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
static BaseCharacteristicManager *characteristics[static_cast<int>(MessageID::_COUNT)];

static BLECharacteristicManager<AlgorithmStrength> algorithmStrengthCharacteristic{MessageID::ALGORITHM_STRENGTH};
static BLECharacteristicManager<bool> isGameOverCharacteristic{MessageID::IS_GAME_OVER};
static BLECharacteristicManager<bool> isPlayerTurnCharacteristic{MessageID::IS_PLAYER_TURN};
static BLECharacteristicManager<std::string> playerLastShotCoordsCharacteristic{MessageID::PLAYER_LAST_SHOT_COORDS};
static BLECharacteristicManager<std::string> playerGridCharacteristic{MessageID::PLAYER_GRID};
static BLECharacteristicManager<std::string> robotLastShotCoordsCharacteristic{MessageID::ROBOT_LAST_SHOT_COORDS};
static BLECharacteristicManager<ShotResult> robotLastShotResultCharacteristic{MessageID::ROBOT_LAST_SHOT_RESULT};

void setup() {
    characteristics[static_cast<int>(MessageID::ALGORITHM_STRENGTH)] = &algorithmStrengthCharacteristic;
    characteristics[static_cast<int>(MessageID::IS_GAME_OVER)] = &isGameOverCharacteristic;
    characteristics[static_cast<int>(MessageID::IS_PLAYER_TURN)] = &isPlayerTurnCharacteristic;
    characteristics[static_cast<int>(MessageID::PLAYER_LAST_SHOT_COORDS)] = &playerLastShotCoordsCharacteristic;
    characteristics[static_cast<int>(MessageID::PLAYER_GRID)] = &playerGridCharacteristic;
    characteristics[static_cast<int>(MessageID::ROBOT_LAST_SHOT_COORDS)] = &robotLastShotCoordsCharacteristic;
    characteristics[static_cast<int>(MessageID::ROBOT_LAST_SHOT_RESULT)] = &robotLastShotResultCharacteristic;

    // put your setup code here, to run once:
    Serial.begin(9600);

    BLEDevice::init(ServerSettings::serverName);
    BLEServer *pServer = BLEDevice::createServer();

    setupGameInfoService(pServer, algorithmStrengthCharacteristic, isGameOverCharacteristic, isPlayerTurnCharacteristic);
    setupPlayerInfoService(pServer, playerGridCharacteristic, playerLastShotCoordsCharacteristic);
    setupRobotInfoService(pServer, robotLastShotCoordsCharacteristic, robotLastShotResultCharacteristic);

    pServer->getAdvertising()->setMinInterval(ServerSettings::advertisingInterval);
    pServer->getAdvertising()->setMaxInterval(ServerSettings::advertisingInterval);
    pServer->getAdvertising()->start();
    pServer->setCallbacks(&serverCallbacks);

    pinMode(onboardDel, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    for (auto characteristic : characteristics) {
        if (characteristic->readAndUpdateData()) {
        }
    }
}
