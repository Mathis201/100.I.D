#pragma once
#include <BLEServer.h>
#include <BluetoothCommunication/BLECharacteristicManager.h>

void setupGameInfoService(BLEServer *pServer,
                          BLECharacteristicManager<AlgorithmStrength> &algorithmStrengthCharacteristic,
                          BLECharacteristicManager<bool> &isGameOverCharacteristic,
                          BLECharacteristicManager<bool> &isPlayerTurnCharacteristic);