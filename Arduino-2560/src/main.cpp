#include "actions/blue.h"
#include "actions/drop_cup.h"
#include "actions/green.h"
#include "actions/orange.h"
#include "actions/pink.h"
#include "actions/yellow.h"
#include "capteurs/beeper.h"
#include "capteurs/capteur_distance_ir.h"
#include "capteurs/capteurcouleur.h"
#include "capteurs/color_leds.h"
#include "capteurs/line_sensor.h"
#include "capteurs/receveur_ir.h"
#include "motors/motor_management.h"
#include "motors/servo_management.h"
#include <LibRobus.h>

#include "constants.h"

enum RobotStates {
    WAIT_FOR_START,
    FOLLOW_LINE,
    DO_COLOR_ACTION,
    FINISH_COLOR,
    DROP_CUP,
    FINISHED
};

constexpr int kColorReadingDelay = 300; // ms

RobotStates currentRobotState;
ActionColors currentColorAction;

unsigned long lastTime = 0;
unsigned long lastColorReadingTime = 0;

void setup() {
    BoardInit();

    initServo();
    initLineSensor();
    initColorLeds();
    initcapteurcouleur();

    currentRobotState = WAIT_FOR_START;
}

void loop() {
    updateMotorInfo(millis());
    timerLed();
    updateBeeper();


    int remoteCommand = getRemoteReadCommand();
    if (remoteCommand == REMOTE_START_COMMAND) {
        requestBeep(2);
    } else if (remoteCommand == REMOTE_WAKE_COMMAND) {
        requestBeep(3);
    }

    ActionColors couleurActuelleLue = ActionColors::NO_COLOR;
    if (millis() - lastColorReadingTime > kColorReadingDelay &&
        (currentRobotState == RobotStates::WAIT_FOR_START || currentRobotState == RobotStates::FOLLOW_LINE)) {
        lastColorReadingTime = millis();
        couleurActuelleLue = lirecouleuractuel();
        if (couleurActuelleLue != ActionColors::NO_COLOR) {
            lightUpLed(couleurActuelleLue);
        }
    }

    switch (currentRobotState) {
    case WAIT_FOR_START: {
        if (ROBUS_IsBumper(REAR) || remoteCommand == REMOTE_START_COMMAND) {
            currentRobotState = FOLLOW_LINE;
        }
        break;
    }

    case FOLLOW_LINE: {
        //Adapt_Curve();

        // if (couleurActuelleLue != ActionColors::NO_COLOR) {
        if (true) {
            currentColorAction = ActionColors::GREEN;
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
    }

    case DO_COLOR_ACTION: {
        bool isColorActionFinished;
        // update current color action and check if finished
        switch (currentColorAction) {
        case ActionColors::BLUE:
            updateBlueAction(remoteCommand);
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
    }

    case FINISH_COLOR: {
        requestBeep(5);
        // TODO: add logic to determine if at end (switch to drop cup) or if keep on following line
        if (true) {
            currentRobotState = DROP_CUP;
            initDropCupAction();
        }

        break;
    }

    case DROP_CUP: {
        updateDropCupAction();
        if (isDropCupActionFinished()) {
            currentRobotState = FINISHED;
        }
        break;
    }

    case FINISHED: {
        currentRobotState = WAIT_FOR_START;
        break;
    }
    }

    delay(10);
    // Serial.print("Loop time: ");
    // Serial.println(millis() - lastTime);
    // Serial.print("Current state: ");
    // Serial.println(currentRobotState);
    lastTime = millis();
}
