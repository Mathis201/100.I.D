#include "../ESP32/lib/BluetoothCommunication/string_characteristic_helper.h"
#include <cassert>
#include <iostream>
#include <string>

void runStringToDataTests() {
    bool err = false;

    // 1. IS_GAME_OVER & IS_PLAYER_TURN (bool)
    assert(stringToData<bool>("0", CharacteristicID::IS_GAME_OVER, err) == false && !err);
    assert(stringToData<bool>("1", CharacteristicID::IS_PLAYER_TURN, err) == true && !err);

    // Error flag resets to false on success
    err = true;
    assert(stringToData<bool>("1", CharacteristicID::IS_GAME_OVER, err) == true && !err);

    // Invalid bool inputs
    assert(stringToData<bool>("", CharacteristicID::IS_GAME_OVER, err) == false && err);
    assert(stringToData<bool>("01", CharacteristicID::IS_GAME_OVER, err) == false && err);
    assert(stringToData<bool>("a", CharacteristicID::IS_PLAYER_TURN, err) == false && err);
    assert(stringToData<bool>("true", CharacteristicID::IS_PLAYER_TURN, err) == false && err);

    // 2. ALGORITHM_STRENGTH (RANDOM=0, NORMAL=1, PROF=2, _COUNT=3)
    assert(stringToData<AlgorithmStrength>("0", CharacteristicID::ALGORITHM_STRENGTH, err) == AlgorithmStrength::RANDOM && !err);
    assert(stringToData<AlgorithmStrength>("1", CharacteristicID::ALGORITHM_STRENGTH, err) == AlgorithmStrength::NORMAL && !err);
    assert(stringToData<AlgorithmStrength>("2", CharacteristicID::ALGORITHM_STRENGTH, err) == AlgorithmStrength::PROF && !err);

    // Out of bounds / invalid AlgorithmStrength
    assert(stringToData<AlgorithmStrength>("-1", CharacteristicID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>("3", CharacteristicID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);   // _COUNT
    assert(stringToData<AlgorithmStrength>("256", CharacteristicID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err); // uint8_t overflow check
    assert(stringToData<AlgorithmStrength>("999999999999999", CharacteristicID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>("PROF", CharacteristicID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);
    assert(stringToData<AlgorithmStrength>("", CharacteristicID::ALGORITHM_STRENGTH, err) == AlgorithmStrength() && err);

    // 3. ROBOT_LAST_SHOT_COORDS & PLAYER_LAST_SHOT_COORDS (std::string)
    std::string validCoord = {kValidRows[0], kValidColumns[0]};
    assert(stringToData<std::string>(validCoord, CharacteristicID::PLAYER_LAST_SHOT_COORDS, err) == validCoord && !err);
    assert(stringToData<std::string>(validCoord, CharacteristicID::ROBOT_LAST_SHOT_COORDS, err) == validCoord && !err);

    // Invalid coords
    assert(stringToData<std::string>("", CharacteristicID::PLAYER_LAST_SHOT_COORDS, err) == "" && err);
    assert(stringToData<std::string>("A", CharacteristicID::PLAYER_LAST_SHOT_COORDS, err) == "" && err);
    assert(stringToData<std::string>("A10", CharacteristicID::PLAYER_LAST_SHOT_COORDS, err) == "" && err);
    assert(stringToData<std::string>("??", CharacteristicID::ROBOT_LAST_SHOT_COORDS, err) == "" && err);

    std::string swappedCoord = {kValidColumns[0], kValidRows[0]};
    if (swappedCoord != validCoord) {
        assert(stringToData<std::string>(swappedCoord, CharacteristicID::ROBOT_LAST_SHOT_COORDS, err) == "" && err);
    }

    // 4. PLAYER_GRID (std::string)
    const int gridLen = kGridSize * kGridSize;
    std::string validGrid(gridLen, '0');
    assert(stringToData<std::string>(validGrid, CharacteristicID::PLAYER_GRID, err) == validGrid && !err);

    // Invalid grid lengths
    assert(stringToData<std::string>("", CharacteristicID::PLAYER_GRID, err) == "" && err);
    assert(stringToData<std::string>(std::string(gridLen - 1, '0'), CharacteristicID::PLAYER_GRID, err) == "" && err);
    assert(stringToData<std::string>(std::string(gridLen + 1, '0'), CharacteristicID::PLAYER_GRID, err) == "" && err);

    // Invalid grid characters (start, middle, end)
    std::string badStart = validGrid;
    badStart[0] = 'x';
    std::string badMid = validGrid;
    badMid[gridLen / 2] = ' ';
    std::string badEnd = validGrid;
    badEnd[gridLen - 1] = '-';
    assert(stringToData<std::string>(badStart, CharacteristicID::PLAYER_GRID, err) == "" && err);
    assert(stringToData<std::string>(badMid, CharacteristicID::PLAYER_GRID, err) == "" && err);
    assert(stringToData<std::string>(badEnd, CharacteristicID::PLAYER_GRID, err) == "" && err);

    // 5. ROBOT_LAST_SHOT_RESULT (HIT=0, SINK=1, MISS=2, _COUNT=3)
    assert(stringToData<ShotResult>("0", CharacteristicID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult::HIT && !err);
    assert(stringToData<ShotResult>("1", CharacteristicID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult::SINK && !err);
    assert(stringToData<ShotResult>("2", CharacteristicID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult::MISS && !err);

    // Out of bounds / invalid ShotResult
    assert(stringToData<ShotResult>("-1", CharacteristicID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err);
    assert(stringToData<ShotResult>("3", CharacteristicID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err); // _COUNT
    assert(stringToData<ShotResult>("256", CharacteristicID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err);
    assert(stringToData<ShotResult>("HIT", CharacteristicID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err);
    assert(stringToData<ShotResult>("", CharacteristicID::ROBOT_LAST_SHOT_RESULT, err) == ShotResult() && err);

    // 6. Type Mismatches & Unknown IDs
    assert(stringToData<int>("1", CharacteristicID::IS_GAME_OVER, err) == 0 && err);
    assert(stringToData<ShotResult>("0", CharacteristicID::ALGORITHM_STRENGTH, err) == ShotResult() && err);
    assert(stringToData<AlgorithmStrength>("0", CharacteristicID::ROBOT_LAST_SHOT_RESULT, err) == AlgorithmStrength() && err);
    assert(stringToData<int>(validGrid, CharacteristicID::PLAYER_GRID, err) == 0 && err);
    assert(stringToData<int>("1", static_cast<CharacteristicID>(999), err) == 0 && err);

    std::cout << "All stringToData tests passed!\n";
}

int main() {
    runStringToDataTests();
}