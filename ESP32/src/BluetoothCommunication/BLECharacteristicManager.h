#pragma once
#include "message_defs.h"
#include <BLECharacteristic.h>

class BaseCharacteristicManager {
public:
    virtual bool readAndUpdateData() = 0;
    virtual std::string getData() = 0;
    virtual void setData(std::string stringRepresentation) = 0;
    virtual MessageID getMessageID() = 0;
};

template <typename SendableType>
class BLECharacteristicManager : public BaseCharacteristicManager {
public:
    BLECharacteristicManager(MessageID messageID);

    // reads the data from the bluetooth characteristic, returns true if there was a change
    bool readAndUpdateData() override;

    // get the current data from the bluetooth characteristic
    std::string getData() override;

    // sets the new data value, writing to the bluetooth characteristic
    void setData(std::string stringRepresentation) override;

    MessageID getMessageID();

    BLECharacteristic *getRawCharacteristic();
    void setCharacteristic(BLECharacteristic *characteristic);

private:
    SendableType data;
    BLECharacteristic *pCharacteristic;
    MessageID messageID;
};

#include "BLECharacteristicManager.tpp"