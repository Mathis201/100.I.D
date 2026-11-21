#include "serial_manager.h"
#include "Arduino.h"
#include "serial_messages.h"

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

void requestSendMessage(String data, SerialMessageID messageID) {
    messages[static_cast<int>(messageID)].setData(data, false);
    messages[static_cast<int>(messageID)].resetSendCount();
}

String getMessage(SerialMessageID messageID, bool &isNewMessage) {
    isNewMessage = messages[static_cast<int>(messageID)].hasReceivedNewData();
    return messages[static_cast<int>(messageID)].getData();
}

void updateSerialManager() {
    while (usedSerial->available() > 0) {
        String read = usedSerial->readStringUntil('\n').c_str();
        Serial.println("DEBUG Received message: " + read);

        bool error;
        SerialMessageContents messageContents = parseStringToMessage(read, error);
        if (error) {
            Serial.println("DEBUG Unable to parse message");
        } else {
            messages[static_cast<int>(messageContents.messageID)].setData(messageContents.data, true);
            Serial.println(String("DEBUG Parsed message to id: ") +
                           static_cast<int>(messageContents.messageID) +
                           " with data: " +
                           messageContents.data);
        }
    }

    if (millis() > printTargetTime) {
        for (int i = 0; i < static_cast<int>(SerialMessageID::_COUNT); i++) {
            if (messages[i].hasMessageToSend()) {
                String message = messages[i].getMessageForSend();
                usedSerial->println(message);
                Serial.println("DEBUG Sent message: " + message);
            }
        }

        printTargetTime = millis() + kSerialPrintDelay;
    }
}
