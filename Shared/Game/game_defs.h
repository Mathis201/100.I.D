#pragma once
#include <stdint.h>

enum class AlgorithmStrength : uint8_t {
    RANDOM,
    NORMAL,
    PROF,
    _COUNT
};

enum class ShotResult : uint8_t {
    HIT,
    SINK,
    MISS,
    _COUNT
};

constexpr int kGridSize = 8;
constexpr char *kValidRows = "ABCDEFGH";
constexpr char *kValidColumns = "12345678";
constexpr int kNumberOfBoats = 5;
constexpr int kBoatLengths[kNumberOfBoats] = {2, 2, 3, 4, 5};