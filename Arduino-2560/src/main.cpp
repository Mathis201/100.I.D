#include "actions/blue.h"
#include "actions/green.h"
#include "actions/orange.h"
#include "actions/pink.h"
#include "actions/yellow.h"
#include <LibRobus.h>

#undef PINK

enum RobotStates {
    WAIT_FOR_START,
    FOLLOW_LINE,
    DO_COLOR_ACTION,
    FINISH_COLOR,
    DROP_CUP,
    FINISHED
};

enum ColorAction {
    BLUE,
    YELLOW,
    GREEN,
    PINK,
    ORANGE,
    NO_ACTION
};

RobotStates currentRobotState;
ColorAction currentColorAction;

void updateCurrentColorAction(ColorAction currentColorAction) {
}

bool isCurrentColorActionFinished(ColorAction currentColorAction) {
}

void setup() {
    BoardInit();

    currentRobotState = WAIT_FOR_START;
}

void loop() {
    // TODO: add check for remote signal (sound)

    switch (currentRobotState) {
    case WAIT_FOR_START:
        // TODO: add check for start remote signal
        break;

    case FOLLOW_LINE:
        // TODO: replace this to trigger when color detected
        if (false) {
            // TODO: currentColorAction = detectedColor
            switch (currentColorAction) {
            case BLUE:
                initBlueAction();
                break;
            case YELLOW:
                initYellowAction();
                break;
            case GREEN:
                initGreenAction();
                break;
            case PINK:
                initPinkAction();
                break;
            case ORANGE:
                initOrangeAction();
                break;
            default:
                break;
            }

            // switch robot state to do action
            currentRobotState = DO_COLOR_ACTION;
        }

        break;

    case DO_COLOR_ACTION:
        bool isColorActionFinished;
        // update current color action and check if finished
        switch (currentColorAction) {
        case BLUE:
            updateBlueAction();
            isColorActionFinished = isBlueActionFinished();
            break;
        case YELLOW:
            updateYellowAction();
            isColorActionFinished = isYellowActionFinished();
            break;
        case GREEN:
            updateGreenAction();
            isColorActionFinished = isGreenActionFinished();
            break;
        case PINK:
            updatePinkAction();
            isColorActionFinished = isPinkActionFinished();
            break;
        case ORANGE:
            updateOrangeAction();
            isColorActionFinished = isOrangeActionFinished();
            break;
        default:
            break;
        }

        // if finished, switch state
        if (isColorActionFinished) {
            currentRobotState = FINISH_COLOR;
        }

        break;

    case FINISH_COLOR:
        currentColorAction = NO_ACTION;

        // TODO: add logic to determine if at end (switch to drop cup) or if keep on following line
        break;

    case DROP_CUP:
        break;

    case FINISHED:
        break;
    }

    delay(20);
}
