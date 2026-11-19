#include "BLECharacteristicManager.h"
#include "ble_setup.h"
#include "characteristic_ids.h"
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
        requestSendMessage(std::to_string(1), SerialMessageID::BLUETOOTH_CONNECTED);
    };
    void onDisconnect(BLEServer *pServer) {
        deviceConnected = false;
        digitalWrite(onboardDel, false);
        pServer->getAdvertising()->stop();
        pServer->getAdvertising()->start();
        requestSendMessage(std::to_string(0), SerialMessageID::BLUETOOTH_CONNECTED);
    }
};

static ServerCallbacks serverCallbacks{};
static BLEServer *pServer;
static BaseCharacteristicManager *characteristics[static_cast<int>(kCharacteristicCount)];

static BLECharacteristicManager<AlgorithmStrength> algorithmStrengthCharacteristic{CharacteristicID::ALGORITHM_STRENGTH};
static BLECharacteristicManager<bool> isGameOverCharacteristic{CharacteristicID::IS_GAME_OVER};
static BLECharacteristicManager<bool> isPlayerTurnCharacteristic{CharacteristicID::IS_PLAYER_TURN};
static BLECharacteristicManager<std::string> playerLastShotCoordsCharacteristic{CharacteristicID::PLAYER_LAST_SHOT_COORDS};
static BLECharacteristicManager<std::string> playerGridCharacteristic{CharacteristicID::PLAYER_GRID};
static BLECharacteristicManager<std::string> robotLastShotCoordsCharacteristic{CharacteristicID::ROBOT_LAST_SHOT_COORDS};
static BLECharacteristicManager<ShotResult> robotLastShotResultCharacteristic{CharacteristicID::ROBOT_LAST_SHOT_RESULT};

void setup() {
    characteristics[static_cast<int>(CharacteristicID::ALGORITHM_STRENGTH)] = &algorithmStrengthCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::IS_GAME_OVER)] = &isGameOverCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::IS_PLAYER_TURN)] = &isPlayerTurnCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::PLAYER_LAST_SHOT_COORDS)] = &playerLastShotCoordsCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::PLAYER_GRID)] = &playerGridCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::ROBOT_LAST_SHOT_COORDS)] = &robotLastShotCoordsCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::ROBOT_LAST_SHOT_RESULT)] = &robotLastShotResultCharacteristic;

    Serial.begin(9600);
    Serial2.begin(9600);

    BLEDevice::init(ServerSettings::serverName);
    BLEServer *pServer = BLEDevice::createServer();

    setupGameInfoService(pServer, algorithmStrengthCharacteristic, isGameOverCharacteristic, isPlayerTurnCharacteristic);
    setupPlayerInfoService(pServer, playerGridCharacteristic, playerLastShotCoordsCharacteristic);
    setupRobotInfoService(pServer, robotLastShotCoordsCharacteristic, robotLastShotResultCharacteristic);

    pServer->getAdvertising()->setMinInterval(ServerSettings::advertisingInterval);
    pServer->getAdvertising()->setMaxInterval(ServerSettings::advertisingInterval);
    pServer->getAdvertising()->start();
    pServer->setCallbacks(&serverCallbacks);

    initSerialManager(Serial);

    pinMode(onboardDel, OUTPUT);
    digitalWrite(onboardDel, LOW);
}

void loop() {
    for (auto characteristic : characteristics) {
        if (characteristic->readAndUpdateData()) {
            requestSendMessage(characteristic->getData(), static_cast<SerialMessageID>(characteristic->getMessageID()));
        }

        std::optional<std::string> newMessage = getNewMessage(static_cast<SerialMessageID>(characteristic->getMessageID()));
        if (newMessage.has_value()) {
            characteristic->setData(newMessage.value());
        }
    }

    updateSerialManager();
}
