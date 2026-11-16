#pragma once
#include "BluetoothCommunication/BLECharacteristicManager.h"
#include <BLEServer.h>

void setupRobotInfoService(BLEServer *pServer,
                           BLECharacteristicManager<std::string> lastShotCoordsCharacteristic,
                           BLECharacteristicManager<ShotResult> lastShotResultCharacteristic);