#pragma once
#include "serial_message_id.h"

// sample serial message: "#3-A4"
constexpr int messageRepetitionCount = 3;

struct SerialMessageContents {
    SerialMessageID messageID;
    String data;
};

inline SerialMessageContents parseStringToMessage(String serialString, bool &error) {
    SerialMessageContents contents;
    error = false;

    serialString.trim();

    if (serialString.length() >= 2 && serialString[1] == '#') {
        serialString = serialString.substring(1);
    }

    // serial message should always be at least 4 chars long and start with # followed by a digit
    if (serialString.length() < 4 || serialString[0] != '#' || !isdigit(serialString[1])) {
        error = true;
        return contents;
    }

    int idLength = -1;
    // find where the dash is, delimits id vs message
    for (unsigned int i = 2; i < serialString.length(); i++) {
        if (serialString[i] == '-') {
            // if found dash, register length and quit
            idLength = i;
            break;
        } else if (!isdigit(serialString[i])) {
            // if found something other than a digit before finding dash, error
            error = true;
            return contents;
        }
    }

    if (idLength == -1) {
        error = true;
        return contents;
    }

    int id = serialString.substring(1, idLength).toInt();
    if (id < 0 || id >= static_cast<int>(SerialMessageID::_COUNT)) {
        error = true;
        return contents;
    }

    String receivedData = serialString.substring(1 + idLength);
    if (receivedData.length() == 0) {
        error = true;
        return contents;
    }

    contents.messageID = static_cast<SerialMessageID>(id);
    contents.data = receivedData;
    return contents;
}

class SerialMessage {
public:
    SerialMessage() : SerialMessage(SerialMessageID::UNKNOWN) {
                      };

    SerialMessage(SerialMessageID messageID) {
        messageContents.messageID = messageID;
        messageContents.data = "";
        countLeftToSend = 0;
        receivedNewData = false;
    }

    // reset count left to send to maximum
    void resetSendCount() {
        countLeftToSend = messageRepetitionCount;
    }

    // check if at least one count left to send
    bool hasMessageToSend() {
        return countLeftToSend != 0;
    }

    // get formatted message to send via serial
    String getMessageForSend() {
        countLeftToSend--;
        return String("##") + static_cast<int>(messageContents.messageID) + "-" + messageContents.data;
    }

    // sets the data for the message, can set the received data flag if want to signify this is new data
    void setData(String data, bool receivedData) {
        if (messageContents.data != data) {
            messageContents.data = data;
            if (receivedData) {
                receivedNewData = true;
            }
        }
    }

    // returns whether the message has been updated since last get
    bool hasReceivedNewData() {
        return receivedNewData;
    }

    // get current message data and remove newData flag because was read
    String getData() {
        receivedNewData = false;
        return messageContents.data;
    }

private:
    SerialMessageContents messageContents;
    int countLeftToSend;
    bool receivedNewData;
};