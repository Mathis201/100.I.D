#pragma once
#include "BLECharacteristicManager.h"
#include <BLEServer.h>

void setupPlayerInfoService(BLEServer *pServer,
                            BLECharacteristicManager<String> &gridCharacteristic,
                            BLECharacteristicManager<String> &lastShotCoordsCharacteristic,
                            BLECharacteristicManager<ShotResult> &lastShotResultCharacteristic);