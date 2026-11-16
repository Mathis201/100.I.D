#pragma once
#include <string>

namespace SerialDataGameInfo {
    static uint8_t algorithmStrength;
    static bool gameOver;
    static bool isPlayerTurn;
};

namespace SerialDataPlayerInfo {
    static uint8_t grid[8][8];
    static std::string lastShotCoords = "A3";
}

namespace SerialDataRobotInfo {
    static std::string lastShotCoords = "B8";
    static uint8_t lastShotResult = 1;
}

void initSerialManager();