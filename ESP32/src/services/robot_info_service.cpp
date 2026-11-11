#include "robot_info_service.h"
#include "ble_setup.h"
#include <BLE2902.h>
#include <BLEServer.h>
#include <string>

static BLECharacteristic *pLastShotCoordsCharacteristic;
static BLECharacteristic *pLastShotResultCharacteristic;

static std::string lastShotCoords = "A3";
static uint8_t lastShotResult = 1;

void setupRobotInfoService(BLEServer *pServer) {
    BLEService *pService = pServer->createService(RobotInfoService::UUID);

    pLastShotCoordsCharacteristic = pService->createCharacteristic(RobotInfoService::LastShotCoordsCharacteristic::UUID,
                                                                   BLECharacteristic::PROPERTY_READ |
                                                                       BLECharacteristic::PROPERTY_NOTIFY);
    pLastShotCoordsCharacteristic->setValue(lastShotCoords);
    pLastShotCoordsCharacteristic->addDescriptor(new BLE2902());

    pLastShotResultCharacteristic = pService->createCharacteristic(RobotInfoService::LastShotResultCharacteristic::UUID,
                                                                   BLECharacteristic::PROPERTY_READ |
                                                                       BLECharacteristic::PROPERTY_NOTIFY);
    pLastShotResultCharacteristic->setValue((uint8_t *)&lastShotResult, sizeof(lastShotResult));
    pLastShotResultCharacteristic->addDescriptor(new BLE2902());

    pService->start();
    pServer->getAdvertising()->addServiceUUID(RobotInfoService::UUID);
}