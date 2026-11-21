#include "game/game_manager.h"
#include "game_defs.h"
#include "serial_manager.h"
#include <LibRobus.h>
#include <Position/PID_follow_line.h>

enum RobotState {
    INITIAL_SETUP,
    WAIT_FOR_BOATS_AND_ALGORITHM,
    WAIT_FOR_PLAYER_SHOT,
    MOVE_TO_PLAYER_SHOT,
    REGISTER_PLAYER_SHOT,
    MANAGE_ROBOT_SHOT,
    GAME_OVER,
    WAIT_FOR_NEXT_GAME
};

RobotState currentRobotState;

void setup() {
    BoardInit();
    Serial2.begin(9600);
    srand(analogRead(A0));

    initLineSensor();
    initSerialManager(Serial2);

    currentRobotState = INITIAL_SETUP;

    // leave time for ESP32 to boot and start server
    delay(2000);
}

void loop() {
    updateSerialManager();

    switch (currentRobotState) {
    case INITIAL_SETUP: {
        resetGameManager();
        requestSendMessage("0", SerialMessageID::IS_GAME_OVER);
        requestSendMessage("1", SerialMessageID::IS_PLAYER_TURN);
        break;
    }

    case WAIT_FOR_BOATS_AND_ALGORITHM: {
        break;
    }

    case WAIT_FOR_PLAYER_SHOT: {
        // requestSendMessage("0", SerialMessageID::IS_PLAYER_TURN);
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
