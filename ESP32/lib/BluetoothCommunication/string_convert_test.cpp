#include "serial_message_id.h"
#include "string_data_helper.h"
#include <Arduino.h>
#include <assert.h>

void runStringToDataTests() {
    bool err = false;

    // 1. IS_GAME_OVER, IS_PLAYER_TURN & IS_BLUETOOTH_CONNECTED (bool)
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

    // Out of bounds / invalid AlgorithmStrength (including whitespace & trailing junk)
    assert(stringToData<AlgorithmStrength>("-1", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>("3", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);   // _COUNT
    assert(stringToData<AlgorithmStrength>("256", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err); // uint8_t overflow check
    assert(stringToData<AlgorithmStrength>("999999999999999", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>(" 1", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>("1 ", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>("1a", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>("PROF", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>("", SerialMessageID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);

    // 3. ROBOT_LAST_SHOT_COORDS & PLAYER_LAST_SHOT_COORDS (String & Coord)
    String validCoord = String(kValidRows[0]) + kValidColumns[0];
    assert(stringToData<String>(validCoord, SerialMessageID::PLAYER_LAST_SHOT_COORDS, err) == validCoord && !err);
    assert(stringToData<String>(validCoord, SerialMessageID::ROBOT_LAST_SHOT_COORDS, err) == validCoord && !err);

    // Coord conversion
    Coord c0 = stringToData<Coord>(validCoord, SerialMessageID::PLAYER_LAST_SHOT_COORDS, err);
    assert(!err && c0.y == (kValidRows[0] - 'A') && c0.x == String(kValidColumns[0]).toInt() - 1);

    String lastValidCoord = String(kValidRows[kGridSize - 1]) + kValidColumns[kGridSize - 1];
    Coord cLast = stringToData<Coord>(lastValidCoord, SerialMessageID::ROBOT_LAST_SHOT_COORDS, err);
    assert(!err && cLast.y == (kValidRows[kGridSize - 1] - 'A') && cLast.x == String(kValidColumns[kGridSize - 1]).toInt() - 1);

    // Invalid coords (String & Coord)
    assert(stringToData<String>("", SerialMessageID::PLAYER_LAST_SHOT_COORDS, err) == "" && err);
    assert(stringToData<String>("A", SerialMessageID::PLAYER_LAST_SHOT_COORDS, err) == "" && err);
    assert(stringToData<String>("A10", SerialMessageID::PLAYER_LAST_SHOT_COORDS, err) == "" && err);
    assert(stringToData<String>("??", SerialMessageID::ROBOT_LAST_SHOT_COORDS, err) == "" && err);

    stringToData<Coord>("", SerialMessageID::PLAYER_LAST_SHOT_COORDS, err);
    assert(err);
    stringToData<Coord>("Z9", SerialMessageID::ROBOT_LAST_SHOT_COORDS, err);
    assert(err);

    String swappedCoord = String(kValidColumns[0]) + kValidRows[0];
    if (swappedCoord != validCoord) {
        assert(stringToData<String>(swappedCoord, SerialMessageID::ROBOT_LAST_SHOT_COORDS, err) == "" && err);
        stringToData<Coord>(swappedCoord, SerialMessageID::ROBOT_LAST_SHOT_COORDS, err);
        assert(err);
    }

    // 4. PLAYER_GRID (String & int (*)[kGridSize])
    const int gridLen = kGridSize * kGridSize;
    String validGrid;
    validGrid.reserve(gridLen);
    for (int i = 0; i < gridLen; i++) {
        validGrid += String(i % 10);
    }
    assert(stringToData<String>(validGrid, SerialMessageID::PLAYER_GRID, err) == validGrid && !err);

    // 2D array pointer conversion
    int (*parsedGrid)[kGridSize] = stringToData<int (*)[kGridSize]>(validGrid, SerialMessageID::PLAYER_GRID, err);
    assert(!err && parsedGrid != nullptr);
    for (int i = 0; i < kGridSize; i++) {
        for (int j = 0; j < kGridSize; j++) {
            assert(parsedGrid[i][j] == (kGridSize * i + j) % 10);
        }
    }

    // Invalid grid lengths (String & pointer)
    assert(stringToData<String>("", SerialMessageID::PLAYER_GRID, err) == "" && err);
    assert(stringToData<String>(validGrid + "0", SerialMessageID::PLAYER_GRID, err) == "" && err);
    assert(stringToData<String>(validGrid.substring(1), SerialMessageID::PLAYER_GRID, err) == "" && err);
    assert(stringToData<int (*)[kGridSize]>("", SerialMessageID::PLAYER_GRID, err) == nullptr && err);
    assert(stringToData<int (*)[kGridSize]>(validGrid + "0", SerialMessageID::PLAYER_GRID, err) == nullptr && err);

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
    assert(stringToData<int (*)[kGridSize]>(badMid, SerialMessageID::PLAYER_GRID, err) == nullptr && err);

    // 5. ROBOT_LAST_SHOT_RESULT (HIT=0, SINK=1, MISS=2, _COUNT=3)
    assert(stringToData<ShotResult>("0", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult::HIT && !err);
    assert(stringToData<ShotResult>("1", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult::SINK && !err);
    assert(stringToData<ShotResult>("2", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult::MISS && !err);

    // Out of bounds / invalid ShotResult
    assert(stringToData<ShotResult>("-1", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err);
    assert(stringToData<ShotResult>("3", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err); // _COUNT
    assert(stringToData<ShotResult>("256", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err);
    assert(stringToData<ShotResult>("0 ", SerialMessageID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err);
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

void runDataToStringTests() {
    // 1. IS_GAME_OVER & IS_PLAYER_TURN (bool)
    assert(dataToString(false, SerialMessageID::IS_GAME_OVER) == "0");
    assert(dataToString(true, SerialMessageID::IS_GAME_OVER) == "1");
    assert(dataToString(false, SerialMessageID::IS_PLAYER_TURN) == "0");
    assert(dataToString(true, SerialMessageID::IS_PLAYER_TURN) == "1");

    // 2. ALGORITHM_STRENGTH
    assert(dataToString(AlgorithmStrength::RANDOM, SerialMessageID::ALGORITHM_STRENGTH) == "0");
    assert(dataToString(AlgorithmStrength::NORMAL, SerialMessageID::ALGORITHM_STRENGTH) == "1");
    assert(dataToString(AlgorithmStrength::PROF, SerialMessageID::ALGORITHM_STRENGTH) == "2");

    // 3. ROBOT_LAST_SHOT_COORDS & PLAYER_LAST_SHOT_COORDS (String & Coord)
    assert(dataToString(String("A1"), SerialMessageID::PLAYER_LAST_SHOT_COORDS) == "A1");
    assert(dataToString(String("C4"), SerialMessageID::ROBOT_LAST_SHOT_COORDS) == "C4");

    Coord c1{0, 0}; // x=0, y=0 -> "A1"
    assert(dataToString(c1, SerialMessageID::PLAYER_LAST_SHOT_COORDS) == "A1");
    Coord c2{3, 2}; // x=3, y=2 -> "C4"
    assert(dataToString(c2, SerialMessageID::ROBOT_LAST_SHOT_COORDS) == "C4");

    // 4. PLAYER_GRID (String & int (*)[kGridSize])
    const int gridLen = kGridSize * kGridSize;
    int grid[kGridSize][kGridSize];
    String expectedGrid;
    expectedGrid.reserve(gridLen);
    for (int i = 0; i < kGridSize; i++) {
        for (int j = 0; j < kGridSize; j++) {
            int val = (kGridSize * i + j) % 10;
            grid[i][j] = val;
            expectedGrid += String(val);
        }
    }
    assert(dataToString(expectedGrid, SerialMessageID::PLAYER_GRID) == expectedGrid);

    int (*gridPtr)[kGridSize] = grid;
    assert(dataToString(gridPtr, SerialMessageID::PLAYER_GRID) == expectedGrid);

    // 5. ROBOT_LAST_SHOT_RESULT
    assert(dataToString(ShotResult::HIT, SerialMessageID::ROBOT_LAST_SHOT_RESULT) == "0");
    assert(dataToString(ShotResult::SINK, SerialMessageID::ROBOT_LAST_SHOT_RESULT) == "1");
    assert(dataToString(ShotResult::MISS, SerialMessageID::ROBOT_LAST_SHOT_RESULT) == "2");

    // 6. Type Mismatches & Unknown IDs
    assert(dataToString(42, SerialMessageID::IS_GAME_OVER) == "");
    assert(dataToString(ShotResult::HIT, SerialMessageID::ALGORITHM_STRENGTH) == "");
    assert(dataToString(AlgorithmStrength::PROF, SerialMessageID::ROBOT_LAST_SHOT_RESULT) == "");
    assert(dataToString(true, static_cast<SerialMessageID>(999)) == "");

    Serial.println("All dataToString tests passed!\n");
}

void runStringConversionTests() {
    runDataToStringTests();
    runStringToDataTests();
}