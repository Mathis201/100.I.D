// #define ALGO_TEST_MODE

#include "game/battleship_algo_test.h"
#include "game/game_manager.h"
#include "game_defs.h"
#include "serial_manager.h"
#include "string_data_helper.h"
#include <LibRobus.h>
#include <Position/PID_follow_line.h>

enum RobotState {
    INITIAL_SETUP,
    WAIT_FOR_GRID_AND_ALGORITHM,
    WAIT_FOR_PLAYER_SHOT,
    MOVE_TO_PLAYER_SHOT,
    REGISTER_PLAYER_SHOT,
    MANAGE_ROBOT_SHOT,
    GAME_OVER,
    WAIT_FOR_NEXT_GAME
};

RobotState currentRobotState;

bool receivedGrid;
bool receivedAlgorithmStrength;

void switchRobotState(RobotState newState) {
    Serial.println(String("DEBUG switched state to ") + (int)newState);
    currentRobotState = newState;
}

void setup() {
    BoardInit();
    srand(analogRead(A0));

#ifdef ALGO_TEST_MODE
    runBattleshipTests();

    while (true) {
        delay(1000);
    }
#endif

    Serial1.begin(9600);

    initLineSensor();
    initSerialManager(Serial1);

    // leave time for ESP32 to boot and start server
    delay(1000);

    switchRobotState(INITIAL_SETUP);
}

void loop() {
    updateSerialManager();

    switch (currentRobotState) {
    case INITIAL_SETUP: {
        resetGameManager();
        receivedGrid = false;
        receivedAlgorithmStrength = false;

        requestSendMessage("0", SerialMessageID::IS_GAME_OVER);
        requestSendMessage("1", SerialMessageID::IS_PLAYER_TURN);

        markAllMessagesAsRead();
        switchRobotState(WAIT_FOR_GRID_AND_ALGORITHM);
        break;
    }

    case WAIT_FOR_GRID_AND_ALGORITHM: {
        bool isNewMessage;
        if (!receivedAlgorithmStrength) {
            String stringAlgorithm = getMessage(SerialMessageID::ALGORITHM_STRENGTH, isNewMessage);
            if (isNewMessage) {
                bool error;
                AlgorithmStrength robotAlgorithmStrength = stringToData<AlgorithmStrength>(stringAlgorithm, SerialMessageID::ALGORITHM_STRENGTH, error);
                if (!error) {
                    Serial.println(String("DEBUG updated algorithm strength to ") + (int)robotAlgorithmStrength);
                    setRobotAlgorithmStrength(robotAlgorithmStrength);
                    receivedAlgorithmStrength = true;
                }
            }
        }

        if (!receivedGrid) {
            String stringGrid = getMessage(SerialMessageID::PLAYER_GRID, isNewMessage);
            if (isNewMessage) {
                Serial.println(stringGrid);
                bool error;
                auto playerGrid = stringToData<int (*)[kGridSize]>(stringGrid, SerialMessageID::PLAYER_GRID, error);
                if (!error) {
                    Serial.println(String("DEBUG updated player grid"));
                    setPlayerCreatedGrid(playerGrid);
                    receivedGrid = true;
                }
            }
        }

        if (receivedGrid && receivedAlgorithmStrength) {
            markAllMessagesAsRead();
            switchRobotState(WAIT_FOR_PLAYER_SHOT);
        }

        break;
    }

    case WAIT_FOR_PLAYER_SHOT: {
        break;
    }

    case MOVE_TO_PLAYER_SHOT: {
        break;
    }

    case REGISTER_PLAYER_SHOT: {

        break;
    }

    case MANAGE_ROBOT_SHOT: {
        // requestSendMessage("G2", SerialMessageID::ROBOT_LAST_SHOT_COORDS);
        // requestSendMessage(String(static_cast<int>(ShotResult::SINK)), SerialMessageID::ROBOT_LAST_SHOT_RESULT);
        // requestSendMessage("1", SerialMessageID::IS_PLAYER_TURN);
        break;
    }

    case GAME_OVER: {
        // requestSendMessage("1", SerialMessageID::IS_GAME_OVER);
        break;
    }

    case WAIT_FOR_NEXT_GAME: {
        break;
    }

    default:
        break;
    }

    // if (ROBUS_IsBumper(REAR)) {
    //     while (true) {
    //         Go_To_Position_XY(5, 3);
    //     }
    // }
}
