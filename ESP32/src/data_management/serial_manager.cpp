#include "serial_manager.h"
#include "Arduino.h"
#include "serial_messages.h"
#include <string>

static constexpr int kSerialPrintDelay = 100;

static unsigned long printTargetTime = 0;

static SerialMessage messages[static_cast<int>(SerialMessageID::_COUNT)];

static HardwareSerial *usedSerial;

void initSerialManager(HardwareSerial &serial) {
    usedSerial = &serial;

    for (int i = 0; i < static_cast<int>(SerialMessageID::_COUNT); i++) {
        messages[i] = SerialMessage(static_cast<SerialMessageID>(i));
    }
}

void requestSendMessage(std::string data, SerialMessageID messageID) {
    messages[static_cast<int>(messageID)].setData(data, false);
    messages[static_cast<int>(messageID)].resetSendCount();
}

std::optional<std::string> getNewMessage(SerialMessageID messageID) {
    if (messages[static_cast<int>(messageID)].hasReceivedNewData()) {
        return messages[static_cast<int>(messageID)].getData();
    }
    return {};
}

void updateSerialManager() {
    while (usedSerial->available() > 0) {
        std::string read = std::string(usedSerial->readStringUntil('\n').c_str());
        Serial.println(("DEBUG Received message: " + read).c_str());

        bool error;
        SerialMessageContents messageContents = parseStringToMessage(read, error);
        if (error) {
            Serial.println("DEBUG Unable to parse message");
        } else {
            messages[static_cast<int>(messageContents.messageID)].setData(messageContents.data, true);
            Serial.println((std::string("DEBUG Parsed message to id: ") +
                            std::to_string(static_cast<int>(messageContents.messageID)) +
                            " with data: " +
                            messageContents.data)
                               .c_str());
        }
    }

    if (millis() > printTargetTime) {
        for (int i = 0; i < static_cast<int>(SerialMessageID::_COUNT); i++) {
            if (messages[i].hasMessageToSend()) {
                std::string message = messages[i].getMessageForSend();
                usedSerial->println(message.c_str());
                Serial.println(("DEBUG Sent message: " + message).c_str());
            }
        }

        printTargetTime = millis() + kSerialPrintDelay;
    }
}
