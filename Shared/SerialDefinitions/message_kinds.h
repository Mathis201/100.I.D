#pragma once

enum class RobotMessageKinds {
    IS_GAME_OVER,
    IS_PLAYER_TURN,
    LAST_SHOT_COORDS,
    LAST_SHOT_RESULT
};

enum PlayerMessageKinds {
    ALGORITHM_STRENGTH,
    GRID,
    LAST_SHOT_COORDS
};