#pragma once
#include "Arduino.h"
#include "game_defs.h"
#include "serial_message_id.h"

namespace detail {
    template <typename T, typename U>
    inline constexpr bool is_same_v = false;

    template <typename T>
    inline constexpr bool is_same_v<T, T> = true;
} // namespace detail

// Helper to strictly parse a base-10 integer without silent 0s or trailing junk
inline bool tryParseInt(const String &str, int &out) {
    if (str.length() == 0 || isspace(static_cast<unsigned char>(str[0]))) {
        return false;
    }
    char *endPtr = nullptr;
    long val = strtol(str.c_str(), &endPtr, 10);
    if (endPtr == str.c_str() || *endPtr != '\0') {
        return false;
    }
    out = static_cast<int>(val);
    return true;
}

template <typename DataType>
DataType stringToData(const String &stringData, SerialMessageID messageID, bool &error) {
    error = false;

    switch (messageID) {
    case SerialMessageID::IS_GAME_OVER:
    case SerialMessageID::IS_NEW_GAME_REQUESTED:
    case SerialMessageID::IS_BLUETOOTH_CONNECTED: {
        if constexpr (detail::is_same_v<DataType, bool>) {
            if (stringData == "0") {
                return false;
            }
            if (stringData == "1") {
                return true;
            }
        }
        break;
    }

    case SerialMessageID::ALGORITHM_STRENGTH: {
        if constexpr (detail::is_same_v<DataType, AlgorithmStrength>) {
            int convertedData = 0;
            if (tryParseInt(stringData, convertedData) &&
                convertedData >= 0 &&
                convertedData < static_cast<int>(AlgorithmStrength::_COUNT)) {
                return static_cast<AlgorithmStrength>(convertedData);
            }
        }
        break;
    }

    case SerialMessageID::ROBOT_LAST_SHOT_COORDS:
    case SerialMessageID::PLAYER_LAST_SHOT_COORDS: {
        if (stringData.length() == 2 &&
            String(kValidRows).indexOf(stringData[0]) != -1 &&
            String(kValidColumns).indexOf(stringData[1]) != -1) {
            if constexpr (detail::is_same_v<DataType, String>) {
                return stringData;
            }

            // if wanted type is coordinate, transform to x and y int positions
            if constexpr (detail::is_same_v<DataType, Coord>) {
                Coord shotCoord;
                // y is row letter code minus 'A' (first row)
                shotCoord.y = stringData[0] - 'A';
                // x is directly an int, offset by 1
                shotCoord.x = String(stringData[1]).toInt() - 1;
                return shotCoord;
            }
        }

        break;
    }

    case SerialMessageID::PLAYER_GRID: {
        constexpr int gridSquareCount = kGridSize * kGridSize;
        // validate that the string is in the correct format
        if (stringData.length() == gridSquareCount) {
            bool allValid = true;
            for (int i = 0; i < gridSquareCount; i++) {
                if (!isdigit(static_cast<unsigned char>(stringData[i]))) {
                    allValid = false;
                    break;
                }
            }
            if (allValid) {
                // if wanted type is string, simply return the string
                if constexpr (detail::is_same_v<DataType, String>) {
                    return stringData;
                }

                // if wanted type is pointer to array (array of arrays)
                if constexpr (detail::is_same_v<DataType, int (*)[kGridSize]> ||
                              detail::is_same_v<DataType, int[kGridSize][kGridSize]>) {
                    static int grid[kGridSize][kGridSize];
                    for (int i = 0; i < kGridSize; i++) {
                        for (int j = 0; j < kGridSize; j++) {
                            grid[i][j] = String(stringData[kGridSize * i + j]).toInt();
                        }
                    }
                    return grid;
                }
            }
        }

        break;
    }

    case SerialMessageID::ROBOT_LAST_SHOT_RESULT:
    case SerialMessageID::PLAYER_LAST_SHOT_RESULT: {
        if constexpr (detail::is_same_v<DataType, ShotResult>) {
            int convertedData = 0;
            if (tryParseInt(stringData, convertedData) &&
                convertedData >= 0 &&
                convertedData < static_cast<int>(ShotResult::_COUNT)) {
                return static_cast<ShotResult>(convertedData);
            }
        }
        break;
    }

    default:
        break;
    }

    error = true;
    return DataType();
}

template <typename DataType>
String dataToString(const DataType &data, SerialMessageID messageID) {
    switch (messageID) {
    case SerialMessageID::IS_GAME_OVER:
    case SerialMessageID::IS_NEW_GAME_REQUESTED:
    case SerialMessageID::IS_BLUETOOTH_CONNECTED:
        if constexpr (detail::is_same_v<DataType, bool>) {
            return String(static_cast<int>(data));
        }
        break;
    case SerialMessageID::ALGORITHM_STRENGTH:
        if constexpr (detail::is_same_v<DataType, AlgorithmStrength>) {
            return String(static_cast<int>(data));
        }
        break;
    case SerialMessageID::ROBOT_LAST_SHOT_COORDS:
    case SerialMessageID::PLAYER_LAST_SHOT_COORDS:
        if constexpr (detail::is_same_v<DataType, String>) {
            return data;
        }
        if constexpr (detail::is_same_v<DataType, Coord>) {
            String shotCoords;
            // A char code plus y -> letter for row
            shotCoords += char(data.y + 'A');
            shotCoords += String(data.x + 1);
            return shotCoords;
        }
        break;
    case SerialMessageID::PLAYER_GRID:
        if constexpr (detail::is_same_v<DataType, String>) {
            return data;
        }
        if constexpr (detail::is_same_v<DataType, int (*)[kGridSize]> ||
                      detail::is_same_v<DataType, int[kGridSize][kGridSize]>) {
            String grid;
            for (int i = 0; i < kGridSize; i++) {
                for (int j = 0; j < kGridSize; j++) {
                    grid += String(data[i][j]);
                }
            }
            return grid;
        }
        break;
    case SerialMessageID::ROBOT_LAST_SHOT_RESULT:
    case SerialMessageID::PLAYER_LAST_SHOT_RESULT:
        if constexpr (detail::is_same_v<DataType, ShotResult>) {
            return String(static_cast<int>(data));
        }
        break;
    default:
        break;
    }

    return "";
}