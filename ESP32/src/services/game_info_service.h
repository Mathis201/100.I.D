#pragma once
#include <BLECharacteristicManager.h>
#include <BLEServer.h>

void setupGameInfoService(BLEServer *pServer,
                          BLECharacteristicManager<AlgorithmStrength> &algorithmStrengthCharacteristic,
                          BLECharacteristicManager<bool> &isGameOverCharacteristic,
                          BLECharacteristicManager<bool> &isNewGameRequestedCharacteristic);