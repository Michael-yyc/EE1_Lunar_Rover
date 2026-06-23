#include <Arduino.h>
#include "rock_classifier.h"

String classify_rock(int ir_lambda, bool ultrasonic_detected, int magnetic_direction) {
    // magnetic_direction:
    //  1  = UP
    // -1  = DOWN
    //  0  = NONE

    if (ir_lambda == 547 && ultrasonic_detected && magnetic_direction == -1) {
        return "BASALTOID";
    }

    if (ir_lambda == 312 && !ultrasonic_detected && magnetic_direction == -1) {
        return "GRAVION";
    }

    if (ir_lambda == 312 && ultrasonic_detected && magnetic_direction == 1) {
        return "REGOLIX";
    }

    if (ir_lambda == 547 && !ultrasonic_detected && magnetic_direction == 1) {
        return "LUNARITE";
    }

    return "UNKNOWN";
}

String magnetic_to_text(int magnetic_direction) {
    if (magnetic_direction == 1) {
        return "UP";
    }
    else if (magnetic_direction == -1) {
        return "DOWN";
    }
    else {
        return "NONE";
    }
}