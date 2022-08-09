//
// Created by Thomas Spycher on 06.08.22.
//

#include "EngineSpeed.h"
#include <Arduino.h>

EngineSpeed::EngineSpeed(Serial_CAN &can): Parameter(can) {
    pid = 0x0C;
    name = "RPM";
    unit = "rpm";
    maximum_value = 16383;
    minimum_value = 0;
    data_bytes = 2;
    previous_value = 10;
}

int EngineSpeed::get_value() {
    return (256*get_a()+get_b())/4;
}