#include "game_info_service.h"
#include "ble_setup.h"
#include "serial_messaging/serial_manager.h"
#include <BLE2902.h>
#include <BLEServer.h>

static BLECharacteristic *pAlgorithmStrengthCharacteristic;
static BLECharacteristic *pIsGameOverCharacteristic;
static BLECharacteristic *pIsPlayerTurnCharacteristic;

void setupGameInfoService(BLEServer *pServer) {
    BLEService *pService = pServer->createService(GameInfoService::UUID);

    pAlgorithmStrengthCharacteristic = pService->createCharacteristic(GameInfoService::AlgorithmStrengthCharacteristic::UUID,
                                                                      BLECharacteristic::PROPERTY_READ |
                                                                          BLECharacteristic::PROPERTY_WRITE);
    pAlgorithmStrengthCharacteristic->setValue((uint8_t *)&SerialDataGameInfo::algorithmStrength, sizeof(SerialDataGameInfo::algorithmStrength));

    pIsGameOverCharacteristic = pService->createCharacteristic(GameInfoService::IsGameOverCharacteristic::UUID,
                                                               BLECharacteristic::PROPERTY_READ |
                                                                   BLECharacteristic::PROPERTY_NOTIFY);
    pIsGameOverCharacteristic->setValue((uint8_t *)&SerialDataGameInfo::gameOver, sizeof(SerialDataGameInfo::gameOver));
    pIsGameOverCharacteristic->addDescriptor(new BLE2902());

    pIsPlayerTurnCharacteristic = pService->createCharacteristic(GameInfoService::IsPlayerTurnCharacteristic::UUID,
                                                                 BLECharacteristic::PROPERTY_READ |
                                                                     BLECharacteristic::PROPERTY_NOTIFY);
    pIsPlayerTurnCharacteristic->setValue((uint8_t *)&SerialDataGameInfo::isPlayerTurn, sizeof(SerialDataGameInfo::isPlayerTurn));
    pIsPlayerTurnCharacteristic->addDescriptor(new BLE2902());

    pService->start();
    pServer->getAdvertising()->addServiceUUID(GameInfoService::UUID);
}