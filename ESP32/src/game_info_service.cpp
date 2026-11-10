#include "ble_setup.h"
#include <BLE2902.h>
#include <BLEServer.h>

static BLECharacteristic *pAlgorithmStrengthCharacteristic;
static BLECharacteristic *pGameOverCharacteristic;

static uint8_t algorithmStrength;
static bool gameOver = false;

void setupGameInfoService(BLEServer *pServer) {
    BLEService *pService = pServer->createService(GameInfoService::UUID, 30);

    pAlgorithmStrengthCharacteristic = pService->createCharacteristic(GameInfoService::AlgorithmStrengthCharacteristic::UUID,
                                                                      BLECharacteristic::PROPERTY_READ |
                                                                          BLECharacteristic::PROPERTY_WRITE);
    pAlgorithmStrengthCharacteristic->setValue((uint8_t *)&algorithmStrength, sizeof(algorithmStrength));

    pGameOverCharacteristic = pService->createCharacteristic(GameInfoService::GameOverCharacteristic::UUID,
                                                             BLECharacteristic::PROPERTY_READ |
                                                                 BLECharacteristic::PROPERTY_NOTIFY);
    pGameOverCharacteristic->setValue((uint8_t *)&gameOver, sizeof(gameOver));
    pGameOverCharacteristic->addDescriptor(new BLE2902());

    pService->start();
    pServer->getAdvertising()->addServiceUUID(GameInfoService::UUID);
}