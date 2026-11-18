#include "BLECharacteristic.h"
#include "BLECharacteristicManager.h"
#include "string_characteristic_helper.h"

template <typename SendableType>
BLECharacteristicManager<SendableType>::BLECharacteristicManager(CharacteristicID messageID) {
    this->messageID = messageID;
}

// general template
template <typename SendableType>
bool BLECharacteristicManager<SendableType>::readAndUpdateData() {
    // get data from the bluetooth characteristic (and cast to desired data type)
    SendableType newData = static_cast<SendableType>(*pCharacteristic->getData());
    // if not the same, update
    if (newData != this->data) {
        this->data = newData;
        return true;
    }
    return false;
}

// only for std::string
template <>
bool inline BLECharacteristicManager<std::string>::readAndUpdateData() {
    std::string newData = pCharacteristic->getValue();
    if (newData != this->data) {
        this->data = newData;
        return true;
    }
    return false;
}

template <typename SendableType>
std::string BLECharacteristicManager<SendableType>::getData() {
    return dataToString<SendableType>(this->data, this->getMessageID());
}

// general template
template <typename SendableType>
void BLECharacteristicManager<SendableType>::setData(std::string stringRepresentation) {
    this->data = stringToData<SendableType>(stringRepresentation, this->getMessageID());
    pCharacteristic->setValue((uint8_t *)&this->data, sizeof(this->data));
}

// only for std::string
template <>
void inline BLECharacteristicManager<std::string>::setData(std::string stringRepresentation) {
    this->data = stringToData<std::string>(stringRepresentation, this->getMessageID());
    // Pass the string directly
    pCharacteristic->setValue(this->data);
}

template <typename SendableType>
CharacteristicID BLECharacteristicManager<SendableType>::getMessageID() {
    return this->messageID;
}

template <typename SendableType>
BLECharacteristic *BLECharacteristicManager<SendableType>::getRawCharacteristic() {
    return this->pCharacteristic;
}

template <typename SendableType>
void BLECharacteristicManager<SendableType>::setCharacteristic(BLECharacteristic *characteristic) {
    this->pCharacteristic = characteristic;
}