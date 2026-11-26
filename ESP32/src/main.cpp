#include "BLECharacteristicManager.h"
#include "ble_setup.h"
#include "characteristic_ids.h"
#include "serial_manager.h"
#include "services/game_info_service.h"
#include "services/player_info_service.h"
#include "services/robot_info_service.h"
#include "string_convert_test.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

constexpr static int onboardDel = 2;

static bool deviceConnected = false;

// Setup callbacks onConnect and onDisconnect
class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        deviceConnected = true;
        digitalWrite(onboardDel, true);
        requestSendMessage(String(1), SerialMessageID::IS_BLUETOOTH_CONNECTED);
    };
    void onDisconnect(BLEServer *pServer) {
        deviceConnected = false;
        digitalWrite(onboardDel, false);
        pServer->getAdvertising()->stop();
        pServer->getAdvertising()->start();
        requestSendMessage(String(0), SerialMessageID::IS_BLUETOOTH_CONNECTED);
    }
};

static ServerCallbacks serverCallbacks{};
static BLEServer *pServer;
static BaseCharacteristicManager *characteristics[static_cast<int>(CharacteristicID::_COUNT)];

static BLECharacteristicManager<AlgorithmStrength> algorithmStrengthCharacteristic{CharacteristicID::ALGORITHM_STRENGTH};
static BLECharacteristicManager<bool> isGameOverCharacteristic{CharacteristicID::IS_GAME_OVER};
static BLECharacteristicManager<bool> isNewGameRequestedCharacteristic{CharacteristicID::IS_NEW_GAME_REQUESTED};
static BLECharacteristicManager<String> playerLastShotCoordsCharacteristic{CharacteristicID::PLAYER_LAST_SHOT_COORDS};
static BLECharacteristicManager<ShotResult> playerLastShotResultCharacteristic{CharacteristicID::PLAYER_LAST_SHOT_RESULT};
static BLECharacteristicManager<String> playerGridCharacteristic{CharacteristicID::PLAYER_GRID};
static BLECharacteristicManager<String> robotLastShotCoordsCharacteristic{CharacteristicID::ROBOT_LAST_SHOT_COORDS};
static BLECharacteristicManager<ShotResult> robotLastShotResultCharacteristic{CharacteristicID::ROBOT_LAST_SHOT_RESULT};

void resetCharacteristicsToDefault() {
    for (auto characteristic : characteristics) {
        characteristic->resetCharacteristicData();
    }
}

void setup() {
    characteristics[static_cast<int>(CharacteristicID::ALGORITHM_STRENGTH)] = &algorithmStrengthCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::IS_GAME_OVER)] = &isGameOverCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::IS_NEW_GAME_REQUESTED)] = &isNewGameRequestedCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::PLAYER_LAST_SHOT_COORDS)] = &playerLastShotCoordsCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::PLAYER_LAST_SHOT_RESULT)] = &playerLastShotResultCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::PLAYER_GRID)] = &playerGridCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::ROBOT_LAST_SHOT_COORDS)] = &robotLastShotCoordsCharacteristic;
    characteristics[static_cast<int>(CharacteristicID::ROBOT_LAST_SHOT_RESULT)] = &robotLastShotResultCharacteristic;

    Serial.begin(9600);
    Serial2.begin(9600);

    BLEDevice::init(ServerSettings::serverName);
    BLEServer *pServer = BLEDevice::createServer();

    setupGameInfoService(pServer, algorithmStrengthCharacteristic, isGameOverCharacteristic, isNewGameRequestedCharacteristic);
    setupPlayerInfoService(pServer, playerGridCharacteristic, playerLastShotCoordsCharacteristic, playerLastShotResultCharacteristic);
    setupRobotInfoService(pServer, robotLastShotCoordsCharacteristic, robotLastShotResultCharacteristic);

    pServer->getAdvertising()->setMinInterval(ServerSettings::advertisingInterval);
    pServer->getAdvertising()->setMaxInterval(ServerSettings::advertisingInterval);
    pServer->getAdvertising()->start();
    pServer->setCallbacks(&serverCallbacks);

    initSerialManager(Serial2);

    resetCharacteristicsToDefault();

    pinMode(onboardDel, OUTPUT);
    digitalWrite(onboardDel, LOW);

    // runStringConversionTests();
}

void loop() {
    for (auto characteristic : characteristics) {
        if (characteristic->readAndUpdateData()) {
            requestSendMessage(characteristic->getData(), static_cast<SerialMessageID>(characteristic->getCharacteristicID()));
        }

        bool isNewMessage;
        String newMessage = getMessage(static_cast<SerialMessageID>(characteristic->getCharacteristicID()), isNewMessage);
        if (isNewMessage) {
            characteristic->setData(newMessage);
        }
    }

    bool isNewMessage;
    String stringResetRequested = getMessage(SerialMessageID::IS_RESET_REQUESTED, isNewMessage);
    if (isNewMessage) {
        bool error;
        bool isResetRequested = stringToData<bool>(stringResetRequested, SerialMessageID::IS_RESET_REQUESTED, error);
        if (isResetRequested && !error) {
            resetCharacteristicsToDefault();
            markAllMessagesAsRead();
            Serial.println(F("DEBUG Reset all Bluetooth characteristics"));
        }
    }

    updateSerialManager();
}
