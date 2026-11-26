#include "Arduino.h"
#include "BLECharacteristic.h"
#include "BLECharacteristicManager.h"
#include "string_data_helper.h"

const std::string kEmptyPlaceholder = "?";

template <typename SendableType>
BLECharacteristicManager<SendableType>::BLECharacteristicManager(CharacteristicID messageID) {
    this->messageID = messageID;
    this->data = std::nullopt;
}

// general template
template <typename SendableType>
bool BLECharacteristicManager<SendableType>::readAndUpdateData() {
    // if empty, skip
    String stringNewData = pCharacteristic->getValue().c_str();
    if (stringNewData == kEmptyPlaceholder.c_str()) {
        // Serial.println(String(F("DEBUG characteristic with ID ")) + (int)this->getCharacteristicID() + F(" has empty string, no update done"));
        return false;
    }

    // get data from the bluetooth characteristic (and cast to desired data type)
    SendableType newData = static_cast<SendableType>(*pCharacteristic->getData());
    // if not the same, update
    if (!this->data.has_value() || newData != this->data.value()) {
        this->data = newData;
        return true;
    }
    return false;
}

// only for String
template <>
bool inline BLECharacteristicManager<String>::readAndUpdateData() {
    // if empty, skip
    String stringNewData = pCharacteristic->getValue().c_str();
    if (stringNewData == kEmptyPlaceholder.c_str()) {
        // Serial.println(String(F("DEBUG characteristic with ID ")) + (int)this->getCharacteristicID() + F(" has empty string, no update done"));
        return false;
    }

    if (!this->data.has_value() || stringNewData != this->data.value()) {
        this->data = stringNewData;
        return true;
    }
    return false;
}

template <typename SendableType>
String BLECharacteristicManager<SendableType>::getData() {
    if (data.has_value()) {
        return dataToString<SendableType>(this->data.value(), static_cast<SerialMessageID>(this->getCharacteristicID()));
    }

    return "";
}

// general template
template <typename SendableType>
void BLECharacteristicManager<SendableType>::setData(String stringRepresentation) {
    bool error;
    SendableType temp = stringToData<SendableType>(stringRepresentation, static_cast<SerialMessageID>(this->getCharacteristicID()), error);
    if (!error) {
        this->data = temp;
        pCharacteristic->setValue((uint8_t *)&this->data.value(), sizeof(this->data.value()));
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
        pCharacteristic->setValue((uint8_t *)this->data.value().c_str(), this->data.value().length());
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
    pCharacteristic->setValue(kEmptyPlaceholder);
    this->data = std::nullopt;
}