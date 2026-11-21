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
static const char *kValidRows = "ABCDEFGH";
static const char *kValidColumns = "12345678";