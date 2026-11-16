#include "player_info_service.h"
#include "BluetoothCommunication/BLECharacteristicManager.h"
#include "ble_setup.h"
#include <BLE2902.h>
#include <BLEServer.h>
#include <string>

void setupPlayerInfoService(BLEServer *pServer,
                            BLECharacteristicManager<std::string> gridCharacteristic,
                            BLECharacteristicManager<std::string> lastShotCoordsCharacteristic) {
    BLEService *pService = pServer->createService(PlayerInfoService::UUID);

    gridCharacteristic.setCharacteristic(pService->createCharacteristic(PlayerInfoService::GridCharacteristic::UUID,
                                                                        BLECharacteristic::PROPERTY_READ |
                                                                            BLECharacteristic::PROPERTY_WRITE));

    lastShotCoordsCharacteristic.setCharacteristic(pService->createCharacteristic(PlayerInfoService::LastShotCoordsCharacteristic::UUID,
                                                                                  BLECharacteristic::PROPERTY_READ |
                                                                                      BLECharacteristic::PROPERTY_WRITE));

    pService->start();
    pServer->getAdvertising()->addServiceUUID(PlayerInfoService::UUID);
}