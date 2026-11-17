#pragma once
#include <string>

// sample serial message: "#3-A4"
constexpr int messageRepetitionCount = 3;

enum class SerialMessageID {
    ALGORITHM_STRENGTH,
    IS_GAME_OVER,
    IS_PLAYER_TURN,
    ROBOT_LAST_SHOT_COORDS,
    ROBOT_LAST_SHOT_RESULT,
    PLAYER_GRID,
    PLAYER_LAST_SHOT_COORDS,
    BLUETOOTH_CONNECTED,
    _COUNT,
    UNKNOWN
};

struct SerialMessageContents {
    SerialMessageID messageID;
    std::string data;
};

static SerialMessageContents parseStringToMessage(std::string serialString, bool &error) {
    SerialMessageContents contents;
    error = false;
    // serial message should always be at least 4 chars long and start with # followed by a digit
    if (serialString.length() < 4 || serialString[0] != '#' || !isdigit(serialString[1])) {
        error = true;
        return contents;
    }

    try {
        int idLength = -1;
        // find where the dash is, delimits id vs message
        for (int i = 2; i < serialString.length(); i++) {
            if (serialString[i] == '-') {
                // if found dash, register length and quit
                idLength = i - 1;
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

        contents.messageID = static_cast<SerialMessageID>(std::stoi(serialString.substr(1, idLength)));
        contents.data = serialString.substr(2 + idLength);
        return contents;
    } catch (...) {
        error = true;
        return contents;
    }
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
    std::string getMessageForSend() {
        countLeftToSend--;
        return "#" + std::to_string(static_cast<int>(messageContents.messageID)) + "-" + messageContents.data;
    }

    // sets the data for the message, can set the received data flag if want to signify this is new data
    void setData(std::string data, bool receivedData) {
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

    // get current message data
    std::string getData() {
        receivedNewData = false;
        return messageContents.data;
    }

private:
    SerialMessageContents messageContents;
    int countLeftToSend;
    bool receivedNewData;
};