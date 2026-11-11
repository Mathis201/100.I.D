#include "player_info_service.h"
#include "ble_setup.h"
#include <BLE2902.h>
#include <BLEServer.h>
#include <string>

static BLECharacteristic *pGridCharacteristic;
static BLECharacteristic *pLastShotCoordsCharacteristic;
static BLECharacteristic *pLastShotResultCharacteristic;

static uint8_t grid[8][8];
static std::string lastShotCoords = "B8";
static uint8_t lastShotResult = 1;

void setupPlayerInfoService(BLEServer *pServer) {
    BLEService *pService = pServer->createService(PlayerInfoService::UUID);

    pGridCharacteristic = pService->createCharacteristic(PlayerInfoService::GridCharacteristic::UUID,
                                                         BLECharacteristic::PROPERTY_READ |
                                                             BLECharacteristic::PROPERTY_WRITE);
    pGridCharacteristic->setValue((uint8_t *)&grid, sizeof(grid));

    pLastShotCoordsCharacteristic = pService->createCharacteristic(PlayerInfoService::LastShotCoordsCharacteristic::UUID,
                                                                   BLECharacteristic::PROPERTY_READ |
                                                                       BLECharacteristic::PROPERTY_WRITE);
    pLastShotCoordsCharacteristic->setValue(lastShotCoords);

    pLastShotResultCharacteristic = pService->createCharacteristic(PlayerInfoService::LastShotResultCharacteristic::UUID,
                                                                   BLECharacteristic::PROPERTY_READ |
                                                                       BLECharacteristic::PROPERTY_NOTIFY);
    pLastShotResultCharacteristic->setValue((uint8_t *)&lastShotResult, sizeof(lastShotResult));
    pLastShotResultCharacteristic->addDescriptor(new BLE2902());

    pService->start();
    pServer->getAdvertising()->addServiceUUID(PlayerInfoService::UUID);
}