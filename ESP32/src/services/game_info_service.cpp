#include "game_info_service.h"
#include "BLECharacteristicManager.h"
#include "ble_setup.h"
#include <BLE2902.h>
#include <BLEServer.h>

void setupGameInfoService(BLEServer *pServer,
                          BLECharacteristicManager<AlgorithmStrength> &algorithmStrengthCharacteristic,
                          BLECharacteristicManager<bool> &isGameOverCharacteristic,
                          BLECharacteristicManager<bool> &isNewGameRequestedCharacteristic) {
    BLEService *pService = pServer->createService(GameInfoService::UUID);

    algorithmStrengthCharacteristic.setCharacteristic(pService->createCharacteristic(GameInfoService::AlgorithmStrengthCharacteristic::UUID,
                                                                                     BLECharacteristic::PROPERTY_READ |
                                                                                         BLECharacteristic::PROPERTY_WRITE));

    isGameOverCharacteristic.setCharacteristic(pService->createCharacteristic(GameInfoService::IsGameOverCharacteristic::UUID,
                                                                              BLECharacteristic::PROPERTY_READ |
                                                                                  BLECharacteristic::PROPERTY_NOTIFY));
    isGameOverCharacteristic.getRawCharacteristic()->addDescriptor(new BLE2902());

    isNewGameRequestedCharacteristic.setCharacteristic(pService->createCharacteristic(GameInfoService::IsNewGameRequestedCharacteristic::UUID,
                                                                                      BLECharacteristic::PROPERTY_READ |
                                                                                          BLECharacteristic::PROPERTY_WRITE));

    pService->start();
    pServer->getAdvertising()->addServiceUUID(GameInfoService::UUID);
}