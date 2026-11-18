#pragma once
#include "characteristic_ids.h"
#include "game_defs.h"
#include "string"

// functions used to convert characteristic data to string and inversely, based on message ID

template <typename DataType>
DataType stringToData(std::string &stringData, CharacteristicID messageID) {
    try {
        switch (messageID) {
        case CharacteristicID::IS_GAME_OVER:
        case CharacteristicID::IS_PLAYER_TURN:
            if constexpr (std::is_same<DataType, bool>::value) {
                return static_cast<bool>(std::stoi(stringData));
            }
            break;
        case CharacteristicID::ALGORITHM_STRENGTH:
            if constexpr (std::is_same<DataType, AlgorithmStrength>::value) {
                return static_cast<AlgorithmStrength>(std::stoi(stringData));
            }
            break;
        case CharacteristicID::ROBOT_LAST_SHOT_COORDS:
        case CharacteristicID::PLAYER_LAST_SHOT_COORDS:
        case CharacteristicID::PLAYER_GRID:
            if constexpr (std::is_same<DataType, std::string>::value) {
                return stringData;
            }
            break;
        case CharacteristicID::ROBOT_LAST_SHOT_RESULT:
            if constexpr (std::is_same<DataType, ShotResult>::value) {
                return static_cast<ShotResult>(std::stoi(stringData));
            }
            break;
        default:
            break;
        }
    } catch (...) {
    }

    return DataType();
}

template <typename DataType>
std::string dataToString(DataType data, CharacteristicID messageID) {
    try {
        switch (messageID) {
        case CharacteristicID::IS_GAME_OVER:
        case CharacteristicID::IS_PLAYER_TURN:
            if constexpr (std::is_same<DataType, bool>::value) {
                return std::to_string(static_cast<int>(data));
            }
            break;
        case CharacteristicID::ALGORITHM_STRENGTH:
            if constexpr (std::is_same<DataType, AlgorithmStrength>::value) {
                return std::to_string(static_cast<int>(data));
            }
            break;
        case CharacteristicID::ROBOT_LAST_SHOT_COORDS:
        case CharacteristicID::PLAYER_LAST_SHOT_COORDS:
        case CharacteristicID::PLAYER_GRID:
            if constexpr (std::is_same<DataType, std::string>::value) {
                return data;
            }
            break;
        case CharacteristicID::ROBOT_LAST_SHOT_RESULT:
            if constexpr (std::is_same<DataType, ShotResult>::value) {
                return std::to_string(static_cast<int>(data));
            }
            break;
        default:
            break;
        }
    } catch (...) {
    }

    return "";
}