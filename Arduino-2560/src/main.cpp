#include "serial_manager.h"
#include <LibRobus.h>
#include <Position/PID_follow_line.h>

void setup() {
    BoardInit();
    Serial2.begin(9600);

    initLineSensor();
    initSerialManager(Serial2);
}

void loop() {
    updateSerialManager();

    // if (ROBUS_IsBumper(REAR)) {
    //     while (true) {
    //         Go_To_Position_XY(5, 3);
    //     }
    // }
}
