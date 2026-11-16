#include "robot_info_service.h"
#include "BluetoothCommunication/BLECharacteristicManager.h"
#include "ble_setup.h"
#include <BLE2902.h>
#include <BLEServer.h>
#include <string>

void setupRobotInfoService(BLEServer *pServer,
                           BLECharacteristicManager<std::string> &lastShotCoordsCharacteristic,
                           BLECharacteristicManager<ShotResult> &lastShotResultCharacteristic) {
    BLEService *pService = pServer->createService(RobotInfoService::UUID);

    lastShotCoordsCharacteristic.setCharacteristic(pService->createCharacteristic(RobotInfoService::LastShotCoordsCharacteristic::UUID,
                                                                                  BLECharacteristic::PROPERTY_READ |
                                                                                      BLECharacteristic::PROPERTY_NOTIFY));
    lastShotCoordsCharacteristic.getRawCharacteristic()->addDescriptor(new BLE2902());
    lastShotCoordsCharacteristic.setData("00");

    lastShotResultCharacteristic.setCharacteristic(pService->createCharacteristic(RobotInfoService::LastShotResultCharacteristic::UUID,
                                                                                  BLECharacteristic::PROPERTY_READ |
                                                                                      BLECharacteristic::PROPERTY_NOTIFY));
    lastShotResultCharacteristic.getRawCharacteristic()->addDescriptor(new BLE2902());
    lastShotResultCharacteristic.setData("0");

    pService->start();
    pServer->getAdvertising()->addServiceUUID(RobotInfoService::UUID);
}