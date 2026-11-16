#include "game_info_service.h"
#include "BluetoothCommunication/BLECharacteristicManager.h"
#include "ble_setup.h"
#include <BLE2902.h>
#include <BLEServer.h>

void setupGameInfoService(BLEServer *pServer,
                          BLECharacteristicManager<AlgorithmStrength> algorithmStrengthCharacteristic,
                          BLECharacteristicManager<bool> isGameOverCharacteristic,
                          BLECharacteristicManager<bool> isPlayerTurnCharacteristic) {
    BLEService *pService = pServer->createService(GameInfoService::UUID);

    algorithmStrengthCharacteristic.setCharacteristic(pService->createCharacteristic(GameInfoService::AlgorithmStrengthCharacteristic::UUID,
                                                                                     BLECharacteristic::PROPERTY_READ |
                                                                                         BLECharacteristic::PROPERTY_WRITE));

    isGameOverCharacteristic.setCharacteristic(pService->createCharacteristic(GameInfoService::IsGameOverCharacteristic::UUID,
                                                                              BLECharacteristic::PROPERTY_READ |
                                                                                  BLECharacteristic::PROPERTY_NOTIFY));
    isGameOverCharacteristic.getRawCharacteristic()->addDescriptor(new BLE2902());

    isPlayerTurnCharacteristic.setCharacteristic(pService->createCharacteristic(GameInfoService::IsPlayerTurnCharacteristic::UUID,
                                                                                BLECharacteristic::PROPERTY_READ |
                                                                                    BLECharacteristic::PROPERTY_NOTIFY));
    isPlayerTurnCharacteristic.getRawCharacteristic()->addDescriptor(new BLE2902());

    pService->start();
    pServer->getAdvertising()->addServiceUUID(GameInfoService::UUID);
}