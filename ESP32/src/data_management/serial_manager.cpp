#include "serial_manager.h"
#include "Arduino.h"
#include "serial_messages.h"
#include <string>

static constexpr int kSerialPrintDelay = 100;

static unsigned long printTargetTime = 0;

static SerialMessage messages[static_cast<int>(SerialMessageID::_COUNT)];

void initSerialManager() {
    Serial2.begin(9600);

    for (int i = 0; i < static_cast<int>(SerialMessageID::_COUNT); i++) {
        messages[i] = SerialMessage(static_cast<SerialMessageID>(i));
    }
}

void requestSendMessage(std::string data, SerialMessageID messageId) {
    messages[static_cast<int>(messageId)].setData(data, false);
    messages[static_cast<int>(messageId)].resetSendCount();
}

void updateSerialManager() {
    if (millis() > printTargetTime) {
        for (int i = 0; i < static_cast<int>(SerialMessageID::_COUNT); i++) {
            if (messages[i].hasMessageToSend()) {
                Serial.println(messages[i].getMessageForSend().c_str());
            }
        }

        printTargetTime = millis() + kSerialPrintDelay;
    }
}
