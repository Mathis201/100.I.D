#include "Arduino.h"
#include "BLECharacteristic.h"
#include "BLECharacteristicManager.h"
#include "string_data_helper.h"

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

// only for String
template <>
bool inline BLECharacteristicManager<String>::readAndUpdateData() {
    String newData = pCharacteristic->getValue().c_str();
    if (newData != this->data) {
        this->data = newData;
        return true;
    }
    return false;
}

template <typename SendableType>
String BLECharacteristicManager<SendableType>::getData() {
    return dataToString<SendableType>(this->data, static_cast<SerialMessageID>(this->getCharacteristicID()));
}

// general template
template <typename SendableType>
void BLECharacteristicManager<SendableType>::setData(String stringRepresentation) {
    bool error;
    SendableType temp = stringToData<SendableType>(stringRepresentation, static_cast<SerialMessageID>(this->getCharacteristicID()), error);
    if (!error) {
        this->data = temp;
        pCharacteristic->setValue((uint8_t *)&this->data, sizeof(this->data));
        Serial.println((String("DEBUG Updated characteristic with id: ") +
                        static_cast<int>(this->getCharacteristicID()) +
                        " from string: " +
                        stringRepresentation));
    } else {
        Serial.println(String("DEBUG Failed to update characteristic with id: ") +
                       static_cast<int>(this->getCharacteristicID()) +
                       " from string: " +
                       stringRepresentation);
    }
}

// only for std::string
template <>
void inline BLECharacteristicManager<String>::setData(String stringRepresentation) {
    bool error;
    String temp = stringToData<String>(stringRepresentation, static_cast<SerialMessageID>(this->getCharacteristicID()), error);
    if (!error) {
        this->data = temp;
        // Pass the string directly
        pCharacteristic->setValue((uint8_t *)this->data.c_str(), this->data.length());
        Serial.println(String("DEBUG Updated characteristic with id: ") +
                       static_cast<int>(this->getCharacteristicID()) +
                       " from string: " +
                       stringRepresentation);
    } else {
        Serial.println(String("DEBUG Failed to update characteristic with id: ") +
                       static_cast<int>(this->getCharacteristicID()) +
                       " from string: " +
                       stringRepresentation);
    }
}

template <typename SendableType>
CharacteristicID BLECharacteristicManager<SendableType>::getCharacteristicID() {
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

template <typename SendableType>
void BLECharacteristicManager<SendableType>::resetCharacteristicData() {
    pCharacteristic->setValue("");
}