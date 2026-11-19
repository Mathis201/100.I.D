#pragma once
#include <string>

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
const std::string kValidRows = "ABCDEFGH";
const std::string kValidColumns = "12345678";