#pragma once
#include "Arduino.h"
#include "serial_messages.h"
#include <string>

void initSerialManager(HardwareSerial &serial);

void updateSerialManager();

void requestSendMessage(std::string data, SerialMessageID messageId);

std::optional<std::string> getNewMessage(SerialMessageID messageID);