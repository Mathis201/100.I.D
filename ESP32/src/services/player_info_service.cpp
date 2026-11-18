#include "player_info_service.h"
#include "BluetoothCommunication/BLECharacteristicManager.h"
#include "ble_setup.h"
#include <BLE2902.h>
#include <BLEServer.h>
#include <string>

void setupPlayerInfoService(BLEServer *pServer,
                            BLECharacteristicManager<std::string> &gridCharacteristic,
                            BLECharacteristicManager<std::string> &lastShotCoordsCharacteristic) {
    BLEService *pService = pServer->createService(PlayerInfoService::UUID);

    gridCharacteristic.setCharacteristic(pService->createCharacteristic(PlayerInfoService::GridCharacteristic::UUID,
                                                                        BLECharacteristic::PROPERTY_READ |
                                                                            BLECharacteristic::PROPERTY_WRITE));
    gridCharacteristic.setData(std::string(64, '0'));

    lastShotCoordsCharacteristic.setCharacteristic(pService->createCharacteristic(PlayerInfoService::LastShotCoordsCharacteristic::UUID,
                                                                                  BLECharacteristic::PROPERTY_READ |
                                                                                      BLECharacteristic::PROPERTY_WRITE));
    lastShotCoordsCharacteristic.setData("00");

    pService->start();
    pServer->getAdvertising()->addServiceUUID(PlayerInfoService::UUID);
}