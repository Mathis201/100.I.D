#pragma once
#include "serial_messages.h"

enum class CharacteristicID {
    ALGORITHM_STRENGTH = static_cast<int>(SerialMessageID::ALGORITHM_STRENGTH),
    IS_GAME_OVER = static_cast<int>(SerialMessageID::IS_GAME_OVER),
    IS_PLAYER_TURN = static_cast<int>(SerialMessageID::IS_PLAYER_TURN),
    ROBOT_LAST_SHOT_COORDS = static_cast<int>(SerialMessageID::ROBOT_LAST_SHOT_COORDS),
    ROBOT_LAST_SHOT_RESULT = static_cast<int>(SerialMessageID::ROBOT_LAST_SHOT_RESULT),
    PLAYER_GRID = static_cast<int>(SerialMessageID::PLAYER_GRID),
    PLAYER_LAST_SHOT_COORDS = static_cast<int>(SerialMessageID::PLAYER_LAST_SHOT_COORDS),
    _COUNT
};