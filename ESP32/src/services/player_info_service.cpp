#include "player_info_service.h"
#include "BLECharacteristicManager.h"
#include "ble_setup.h"
#include <BLE2902.h>
#include <BLEServer.h>
#include <string>

void setupPlayerInfoService(BLEServer *pServer,
                            BLECharacteristicManager<String> &gridCharacteristic,
                            BLECharacteristicManager<String> &lastShotCoordsCharacteristic) {
    BLEService *pService = pServer->createService(PlayerInfoService::UUID);

    gridCharacteristic.setCharacteristic(pService->createCharacteristic(PlayerInfoService::GridCharacteristic::UUID,
                                                                        BLECharacteristic::PROPERTY_READ |
                                                                            BLECharacteristic::PROPERTY_WRITE));
    gridCharacteristic.setData("0000000000000000000000000000000000000000000000000000000000000000");

    lastShotCoordsCharacteristic.setCharacteristic(pService->createCharacteristic(PlayerInfoService::LastShotCoordsCharacteristic::UUID,
                                                                                  BLECharacteristic::PROPERTY_READ |
                                                                                      BLECharacteristic::PROPERTY_WRITE));
    lastShotCoordsCharacteristic.setData("A1");

    pService->start();
    pServer->getAdvertising()->addServiceUUID(PlayerInfoService::UUID);
}