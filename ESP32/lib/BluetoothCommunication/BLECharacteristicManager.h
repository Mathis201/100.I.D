#pragma once
#include "Arduino.h"
#include "characteristic_ids.h"
#include <BLECharacteristic.h>

class BaseCharacteristicManager {
public:
    virtual bool readAndUpdateData() = 0;
    virtual String getData() = 0;
    virtual void setData(String stringRepresentation) = 0;
    virtual CharacteristicID getCharacteristicID() = 0;
    virtual void resetCharacteristicData();
};

template <typename SendableType>
class BLECharacteristicManager : public BaseCharacteristicManager {
public:
    BLECharacteristicManager(CharacteristicID messageID);

    // reads the data from the bluetooth characteristic, returns true if there was a change
    bool readAndUpdateData() override;

    // get the current data from the bluetooth characteristic
    String getData() override;

    // sets the new data value, writing to the bluetooth characteristic
    void setData(String stringRepresentation) override;

    CharacteristicID getCharacteristicID() override;

    BLECharacteristic *getRawCharacteristic();
    void setCharacteristic(BLECharacteristic *characteristic);

    void resetCharacteristicData() override;

private:
    SendableType data;
    BLECharacteristic *pCharacteristic;
    CharacteristicID messageID;
};

#include "BLECharacteristicManager.tpp"