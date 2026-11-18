#pragma once
#include "serial_messages.h"
#include <string>

void initSerialManager();

void updateSerialManager();

void requestSendMessage(std::string data, SerialMessageID messageId);