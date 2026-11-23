#pragma once
#include "Arduino.h"
#include "serial_messages.h"

void initSerialManager(HardwareSerial &serial);

void updateSerialManager();

void requestSendMessage(String data, SerialMessageID messageId);

String getMessage(SerialMessageID messageID, bool &isNewMessage);

void markAllMessagesAsRead();