#include "game_info_service.h"
#include "BLECharacteristicManager.h"
#include "ble_setup.h"
#include <BLE2902.h>
#include <BLEServer.h>

void setupGameInfoService(BLEServer *pServer,
                          BLECharacteristicManager<AlgorithmStrength> &algorithmStrengthCharacteristic,
                          BLECharacteristicManager<bool> &isGameOverCharacteristic,
                          BLECharacteristicManager<bool> &isPlayerTurnCharacteristic) {
    BLEService *pService = pServer->createService(GameInfoService::UUID);

    algorithmStrengthCharacteristic.setCharacteristic(pService->createCharacteristic(GameInfoService::AlgorithmStrengthCharacteristic::UUID,
                                                                                     BLECharacteristic::PROPERTY_READ |
                                                                                         BLECharacteristic::PROPERTY_WRITE));
    algorithmStrengthCharacteristic.setData("0");

    isGameOverCharacteristic.setCharacteristic(pService->createCharacteristic(GameInfoService::IsGameOverCharacteristic::UUID,
                                                                              BLECharacteristic::PROPERTY_READ |
                                                                                  BLECharacteristic::PROPERTY_NOTIFY));
    isGameOverCharacteristic.getRawCharacteristic()->addDescriptor(new BLE2902());
    isGameOverCharacteristic.setData("0");

    isPlayerTurnCharacteristic.setCharacteristic(pService->createCharacteristic(GameInfoService::IsPlayerTurnCharacteristic::UUID,
                                                                                BLECharacteristic::PROPERTY_READ |
                                                                                    BLECharacteristic::PROPERTY_NOTIFY));
    isPlayerTurnCharacteristic.getRawCharacteristic()->addDescriptor(new BLE2902());
    isPlayerTurnCharacteristic.setData("0");

    pService->start();
    pServer->getAdvertising()->addServiceUUID(GameInfoService::UUID);
}