#include "serial_message_id.h"
#include "string_data_helper.h"
#include <Arduino.h>

void runStringToDataTests() {
    bool err = false;

    // 1. IS_GAME_OVER & IS_PLAYER_TURN (bool)
    assert(stringToData<bool>("0", SerialMessageID::IS_GAME_OVER, err) == false && !err);
    assert(stringToData<bool>("1", SerialMessageID::IS_PLAYER_TURN, err) == true && !err);
    assert(stringToData<bool>("1", SerialMessageID::IS_BLUETOOTH_CONNECTED, err) == true && !err);

    // Error flag resets to false on success
    err = true;
    assert(stringToData<bool>("1", SerialMessageID::IS_GAME_OVER, err) == true && !err);

    // Invalid bool inputs
    assert(stringToData<bool>("", SerialMessageID::IS_GAME_OVER, err) == false && err);
    assert(stringToData<bool>("01", SerialMessageID::IS_GAME_OVER, err) == false && err);
    assert(stringToData<bool>("a", SerialMessageID::IS_PLAYER_TURN, err) == false && err);
    assert(stringToData<bool>("true", SerialMessageID::IS_PLAYER_TURN, err) == false && err);

    // 2. ALGORITHM_STRENGTH (RANDOM=0, NORMAL=1, PROF=2, _COUNT=3)
    assert(stringToData<AlgorithmStrength>("0", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength::RANDOM && !err);
    assert(stringToData<AlgorithmStrength>("1", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength::NORMAL && !err);
    assert(stringToData<AlgorithmStrength>("2", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength::PROF && !err);

    // Out of bounds / invalid AlgorithmStrength
    assert(stringToData<AlgorithmStrength>("-1", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>("3", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);   // _COUNT
    assert(stringToData<AlgorithmStrength>("256", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err); // uint8_t overflow check
    assert(stringToData<AlgorithmStrength>("999999999999999", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>("PROF", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>("", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);

    // 3. ROBOT_LAST_SHOT_COORDS & PLAYER_LAST_SHOT_COORDS (String)
    String validCoord = String(kValidRows[0]) + kValidColumns[0];
    assert(stringToData<String>(validCoord, SerialMessageID::PLAYER_LAST_SHOT_COORDS, err) == validCoord && !err);
    assert(stringToData<String>(validCoord, SerialMessageID::ROBOT_LAST_SHOT_COORDS, err) == validCoord && !err);

    // Invalid coords
    assert(stringToData<String>("", SerialMessageID::PLAYER_LAST_SHOT_COORDS, err) == "" && err);
    assert(stringToData<String>("A", SerialMessageID::PLAYER_LAST_SHOT_COORDS, err) == "" && err);
    assert(stringToData<String>("A10", SerialMessageID::PLAYER_LAST_SHOT_COORDS, err) == "" && err);
    assert(stringToData<String>("??", SerialMessageID::ROBOT_LAST_SHOT_COORDS, err) == "" && err);

    String swappedCoord = String(kValidColumns[0]) + kValidRows[0];
    if (swappedCoord != validCoord) {
        assert(stringToData<String>(swappedCoord, SerialMessageID::ROBOT_LAST_SHOT_COORDS, err) == "" && err);
    }

    // 4. PLAYER_GRID (String)
    const int gridLen = kGridSize * kGridSize;
    String validGrid("0000000000000000000000000000000000000000000000000000000000000000");
    assert(stringToData<String>(validGrid, SerialMessageID::PLAYER_GRID, err) == validGrid && !err);

    // Invalid grid lengths
    assert(stringToData<String>("", SerialMessageID::PLAYER_GRID, err) == "" && err);
    assert(stringToData<String>(validGrid + "0", SerialMessageID::PLAYER_GRID, err) == "" && err);
    assert(stringToData<String>(validGrid.substring(1), SerialMessageID::PLAYER_GRID, err) == "" && err);

    // Invalid grid characters (start, middle, end)
    String badStart = validGrid;
    badStart[0] = 'x';
    String badMid = validGrid;
    badMid[gridLen / 2] = ' ';
    String badEnd = validGrid;
    badEnd[gridLen - 1] = '-';
    assert(stringToData<String>(badStart, SerialMessageID::PLAYER_GRID, err) == "" && err);
    assert(stringToData<String>(badMid, SerialMessageID::PLAYER_GRID, err) == "" && err);
    assert(stringToData<String>(badEnd, SerialMessageID::PLAYER_GRID, err) == "" && err);

    // 5. ROBOT_LAST_SHOT_RESULT (HIT=0, SINK=1, MISS=2, _COUNT=3)
    assert(stringToData<ShotResult>("0", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult::HIT && !err);
    assert(stringToData<ShotResult>("1", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult::SINK && !err);
    assert(stringToData<ShotResult>("2", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult::MISS && !err);

    // Out of bounds / invalid ShotResult
    assert(stringToData<ShotResult>("-1", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err);
    assert(stringToData<ShotResult>("3", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err); // _COUNT
    assert(stringToData<ShotResult>("256", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err);
    assert(stringToData<ShotResult>("HIT", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err);
    assert(stringToData<ShotResult>("", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err);

    // 6. Type Mismatches & Unknown IDs
    assert(stringToData<int>("1", SerialMessageID::IS_GAME_OVER, err) == 0 && err);
    assert(stringToData<ShotResult>("0", SerialMessageID::ALGORITHM_STRENGTH, err) == ShotResult() && err);
    assert(stringToData<AlgorithmStrength>("0", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == AlgorithmStrength() && err);
    assert(stringToData<int>(validGrid, SerialMessageID::PLAYER_GRID, err) == 0 && err);
    assert(stringToData<int>("1", static_cast<SerialMessageID>(999), err) == 0 && err);

    Serial.println("All stringToData tests passed!\n");
}
