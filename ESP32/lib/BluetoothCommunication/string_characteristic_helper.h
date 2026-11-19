#pragma once
#include "characteristic_ids.h"
#include "game_defs.h"
#include "string"

// functions used to convert characteristic data to string and inversely, based on message ID

template <typename DataType>
DataType stringToData(const std::string &stringData, CharacteristicID messageID, bool &error) {
    error = false;
    try {
        switch (messageID) {
        case CharacteristicID::IS_GAME_OVER:
        case CharacteristicID::IS_PLAYER_TURN: {
            if constexpr (std::is_same<DataType, bool>::value) {
                if (stringData.length() == 1) {
                    return static_cast<bool>(std::stoi(stringData));
                }
            }
            break;
        }

        case CharacteristicID::ALGORITHM_STRENGTH: {
            if constexpr (std::is_same<DataType, AlgorithmStrength>::value) {
                int convertedData = std::stoi(stringData);
                if (convertedData >= 0 && convertedData < static_cast<int>(AlgorithmStrength::_COUNT)) {
                    return static_cast<AlgorithmStrength>(convertedData);
                }
            }
            break;
        }

        case CharacteristicID::ROBOT_LAST_SHOT_COORDS:
        case CharacteristicID::PLAYER_LAST_SHOT_COORDS: {
            // length of two for coordinates (A1)
            if constexpr (std::is_same<DataType, std::string>::value) {
                // make sure that information is coordinates
                if (stringData.length() == 2 &&
                    std::string(kValidRows).find(stringData[0]) != std::string::npos &&
                    std::string(kValidColumns).find(stringData[1]) != std::string::npos) {
                    return stringData;
                }
            }
            break;
        }

        case CharacteristicID::PLAYER_GRID: {
            int gridSquareCount = kGridSize * kGridSize;
            if constexpr (std::is_same<DataType, std::string>::value) {
                if (stringData.length() == gridSquareCount) {
                    bool allValid = true;
                    for (int i = 0; i < gridSquareCount; i++) {
                        if (!isdigit(stringData[i])) {
                            allValid = false;
                            break;
                        }
                    }

                    if (allValid) {
                        return stringData;
                    }
                }
            }
            break;
        }

        case CharacteristicID::ROBOT_LAST_SHOT_RESULT: {
            if constexpr (std::is_same<DataType, ShotResult>::value) {
                int convertedData = std::stoi(stringData);
                if (convertedData >= 0 && convertedData < static_cast<int>(ShotResult::_COUNT)) {
                    return static_cast<ShotResult>(convertedData);
                }
            }
            break;
        }

        default:
            break;
        }
    } catch (...) {
    }

    error = true;
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