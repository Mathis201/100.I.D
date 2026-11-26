// #define ALGO_TEST_MODE

#include "game/battleship_algo_test.h"
#include "game/game_manager.h"
#include "game_defs.h"
#include "motors/servo_management.h"
#include "serial_manager.h"
#include "string_data_helper.h"
#include <LibRobus.h>
#include <MINI_MP3/audio.h>
#define OUTPUT_LED_PIN 53
#include <Position/PID_follow_line.h>
#include <Position/Position_Grille.h>

enum RobotState {
    WAIT_FOR_NEXT_GAME,
    INITIAL_SETUP,
    WAIT_FOR_GRID_AND_ALGORITHM,
    WAIT_FOR_PLAYER_SHOT,
    MOVE_TO_PLAYER_SHOT,
    REGISTER_PLAYER_SHOT,
    MANAGE_ROBOT_SHOT,
    GAME_OVER,
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
    initServo();

    // leave time for ESP32 to boot and start server
    // delay(1000);

    switchRobotState(WAIT_FOR_NEXT_GAME);
}

void loop() {

    if(ROBUS_IsBumper(RIGHT)){

        // Request_Reach_Position_XY(4,4);


        while(true){
            Follow_line(black);
            // Update_Reach_Position_XY();
        }
    }

    // updateSerialManager();

    // switch (currentRobotState) {
    // case WAIT_FOR_NEXT_GAME: {
    //     // wait for next game serial message
    //     bool isNewMessage;
    //     String stringRequestGame = getMessage(SerialMessageID::IS_NEW_GAME_REQUESTED, isNewMessage);
    //     if (isNewMessage) {
    //         bool error;
    //         auto gameRequested = stringToData<bool>(stringRequestGame, SerialMessageID::IS_NEW_GAME_REQUESTED, error);
    //         if (!error) {
    //             Serial.println(String(F("DEBUG received game request: ")) + (int)gameRequested);

    //             if (gameRequested) {
    //                 switchRobotState(INITIAL_SETUP);
    //             }
    //         }
    //     }
    //     break;
    // }

    // case INITIAL_SETUP: {
    //     resetGameManager();
    //     receivedGrid = false;
    //     receivedAlgorithmStrength = false;
    //     requestSendMessage("1", SerialMessageID::IS_RESET_REQUESTED); // ask ESP to reset bluetooth characteristics
    //     requestSendMessage("0", SerialMessageID::IS_GAME_OVER);

    //     markAllMessagesAsRead();
    //     switchRobotState(WAIT_FOR_GRID_AND_ALGORITHM);
    //     break;
    // }

    // case WAIT_FOR_GRID_AND_ALGORITHM: {
    //     bool isNewMessage;
    //     if (!receivedAlgorithmStrength) {
    //         String stringAlgorithm = getMessage(SerialMessageID::ALGORITHM_STRENGTH, isNewMessage);
    //         if (isNewMessage) {
    //             bool error;
    //             AlgorithmStrength robotAlgorithmStrength = stringToData<AlgorithmStrength>(stringAlgorithm, SerialMessageID::ALGORITHM_STRENGTH, error);
    //             if (!error) {
    //                 Serial.println(String(F("DEBUG updated algorithm strength to ")) + (int)robotAlgorithmStrength);
    //                 setRobotAlgorithmStrength(robotAlgorithmStrength);
    //                 receivedAlgorithmStrength = true;
    //             }
    //         }
    //     }

    //     if (!receivedGrid) {
    //         String stringGrid = getMessage(SerialMessageID::PLAYER_GRID, isNewMessage);
    //         if (isNewMessage) {
    //             bool error;
    //             auto playerGrid = stringToData<int (*)[kGridSize]>(stringGrid, SerialMessageID::PLAYER_GRID, error);
    //             if (!error) {
    //                 Serial.println(String(F("DEBUG updated player grid")));
    //                 setPlayerCreatedGrid(playerGrid);
    //                 receivedGrid = true;
    //             }
    //         }
    //     }

    //     if (receivedGrid && receivedAlgorithmStrength) {
    //         markAllMessagesAsRead();
    //         switchRobotState(WAIT_FOR_PLAYER_SHOT);
    //     }

    //     break;
    // }

    // case WAIT_FOR_PLAYER_SHOT: {
    //     bool isNewMessage;
    //     String stringPlayerShot = getMessage(SerialMessageID::PLAYER_LAST_SHOT_COORDS, isNewMessage);
    //     if (isNewMessage) {
    //         bool error;
    //         auto playerShot = stringToData<Coord>(stringPlayerShot, SerialMessageID::PLAYER_LAST_SHOT_COORDS, error);
    //         if (!error) {
    //             // TODO: evaluate player shot, pick up colored pin from dropper
    //         }
    //     }
    // }

    // case MOVE_TO_PLAYER_SHOT: {
    //     // TODO: move from current position to player shot
    //     break;
    // }

    // case REGISTER_PLAYER_SHOT: {
    //     /* TODO:
    //     drop pin from dropper,
    //     play sound,
    //     send shot result,
    //     decide if game is over
    //     */
    //     break;
    // }

    // case MANAGE_ROBOT_SHOT: {
    //     /* TODO:
    //     generate robot shot coords,
    //     evaluate shot,
    //     send both info to player,
    //     decide if game is over
    //     */

    //     // requestSendMessage("G2", SerialMessageID::ROBOT_LAST_SHOT_COORDS);
    //     // requestSendMessage(String(static_cast<int>(ShotResult::SINK)), SerialMessageID::ROBOT_LAST_SHOT_RESULT);
    //     // requestSendMessage("1", SerialMessageID::IS_NEW_GAME_REQUESTED);
    //     break;
    // }

    // case GAME_OVER: {
    //     // TODO: send game over message, play sound?
    //     // requestSendMessage("1", SerialMessageID::IS_GAME_OVER);
    //     break;
    // }

    // default:
    //     break;
    // }
}
