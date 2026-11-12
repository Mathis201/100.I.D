#include "player_info_service.h"
#include "ble_setup.h"
#include "serial_messaging/serial_manager.h"
#include <BLE2902.h>
#include <BLEServer.h>
#include <string>

static BLECharacteristic *pGridCharacteristic;
static BLECharacteristic *pLastShotCoordsCharacteristic;

void setupPlayerInfoService(BLEServer *pServer) {
    BLEService *pService = pServer->createService(PlayerInfoService::UUID);

    pGridCharacteristic = pService->createCharacteristic(PlayerInfoService::GridCharacteristic::UUID,
                                                         BLECharacteristic::PROPERTY_READ |
                                                             BLECharacteristic::PROPERTY_WRITE);
    pGridCharacteristic->setValue((uint8_t *)&SerialDataPlayerInfo::grid, sizeof(SerialDataPlayerInfo::grid));

    pLastShotCoordsCharacteristic = pService->createCharacteristic(PlayerInfoService::LastShotCoordsCharacteristic::UUID,
                                                                   BLECharacteristic::PROPERTY_READ |
                                                                       BLECharacteristic::PROPERTY_WRITE);
    pLastShotCoordsCharacteristic->setValue(SerialDataPlayerInfo::lastShotCoords);

    pService->start();
    pServer->getAdvertising()->addServiceUUID(PlayerInfoService::UUID);
}