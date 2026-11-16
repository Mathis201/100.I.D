#include "robot_info_service.h"
#include "ble_setup.h"
#include "serial_messaging/serial_manager.h"
#include <BLE2902.h>
#include <BLEServer.h>
#include <string>

static BLECharacteristic *pLastShotCoordsCharacteristic;
static BLECharacteristic *pLastShotResultCharacteristic;

void setupRobotInfoService(BLEServer *pServer) {
    BLEService *pService = pServer->createService(RobotInfoService::UUID);

    pLastShotCoordsCharacteristic = pService->createCharacteristic(RobotInfoService::LastShotCoordsCharacteristic::UUID,
                                                                   BLECharacteristic::PROPERTY_READ |
                                                                       BLECharacteristic::PROPERTY_NOTIFY);
    pLastShotCoordsCharacteristic->setValue(SerialDataRobotInfo::lastShotCoords);
    pLastShotCoordsCharacteristic->addDescriptor(new BLE2902());

    pLastShotResultCharacteristic = pService->createCharacteristic(RobotInfoService::LastShotResultCharacteristic::UUID,
                                                                   BLECharacteristic::PROPERTY_READ |
                                                                       BLECharacteristic::PROPERTY_NOTIFY);
    pLastShotResultCharacteristic->setValue((uint8_t *)&SerialDataRobotInfo::lastShotResult, sizeof(SerialDataRobotInfo::lastShotResult));
    pLastShotResultCharacteristic->addDescriptor(new BLE2902());

    pService->start();
    pServer->getAdvertising()->addServiceUUID(RobotInfoService::UUID);
}