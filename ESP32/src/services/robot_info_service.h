#pragma once
#include "BLECharacteristicManager.h"
#include <BLEServer.h>

void setupRobotInfoService(BLEServer *pServer,
                           BLECharacteristicManager<String> &lastShotCoordsCharacteristic,
                           BLECharacteristicManager<ShotResult> &lastShotResultCharacteristic);