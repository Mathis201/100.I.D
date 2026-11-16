#pragma once
#include "BluetoothCommunication/BLECharacteristicManager.h"
#include <BLEServer.h>

void setupPlayerInfoService(BLEServer *pServer,
                            BLECharacteristicManager<std::string> gridCharacteristic,
                            BLECharacteristicManager<std::string> lastShotCoordsCharacteristic);