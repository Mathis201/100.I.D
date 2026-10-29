#include "actions/blue.h"
#include "actions/green.h"
#include "actions/orange.h"
#include "actions/pink.h"
#include "actions/yellow.h"
#include "capteurs/color_leds.h"
#include "capteurs/line_sensor.h"
#include "capteurs/receveur_ir.h"
#include <LibRobus.h>
#include "capteurs/capteurcouleur.h"

#include "constants.h"

#define REMOTE_START_COMMAND 1
#define REMOTE_WAKE_COMMAND 2

enum RobotStates {
    WAIT_FOR_START,
    FOLLOW_LINE,
    DO_COLOR_ACTION,
    FINISH_COLOR,
    DROP_CUP,
    FINISHED
};

RobotStates currentRobotState;
ActionColors currentColorAction;

void beep(int count) {
    for (int i = 0; i < count; i++) {
        AX_BuzzerON();
        delay(75);
        AX_BuzzerOFF();
        delay(75);
    }
}

void setup() {
    BoardInit();
    initLineSensor();
    initColorLeds();

    initcapteurcouleur();
    currentRobotState = WAIT_FOR_START;
}

void loop() {
    Serial.println(lirecouleuractuel());
    int remoteCommand = getRemoteReadCommand();
    if (remoteCommand == REMOTE_START_COMMAND) {
        beep(2);
    } else if (remoteCommand == REMOTE_WAKE_COMMAND) {
        beep(3);
    }

    lightUpLed(ActionColors::YELLOW);
    getNormalizedLineReading();

    switch (currentRobotState) {
    case WAIT_FOR_START:
        // TODO: add check for start remote signal
        break;

    case FOLLOW_LINE:
        // TODO: replace this to trigger when color detected
        if (false) {
            // TODO: currentColorAction = detectedColor
            switch (currentColorAction) {
            case ActionColors::BLUE:
                initBlueAction();
                break;
            case ActionColors::YELLOW:
                initYellowAction();
                break;
            case ActionColors::GREEN:
                initGreenAction();
                break;
            case ActionColors::PINK_:
                initPinkAction();
                break;
            case ActionColors::ORANGE:
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
        case ActionColors::BLUE:
            updateBlueAction();
            isColorActionFinished = isBlueActionFinished();
            break;
        case ActionColors::YELLOW:
            updateYellowAction();
            isColorActionFinished = isYellowActionFinished();
            break;
        case ActionColors::GREEN:
            updateGreenAction();
            isColorActionFinished = isGreenActionFinished();
            break;
        case ActionColors::PINK_:
            updatePinkAction();
            isColorActionFinished = isPinkActionFinished();
            break;
        case ActionColors::ORANGE:
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
        // TODO: add logic to determine if at end (switch to drop cup) or if keep on following line
        break;

    case DROP_CUP:
        break;

    case FINISHED:
        break;
    }
    delay(20);
}
