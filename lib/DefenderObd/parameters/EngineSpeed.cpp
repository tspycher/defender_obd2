//
// Created by Thomas Spycher on 06.08.22.
//

#include "EngineSpeed.h"
#include <Arduino.h>

EngineSpeed::EngineSpeed(Serial_CAN &can) : Parameter(can) {
    EngineSpeed::pid = 0x0C;
    EngineSpeed::name = "RPM";
    EngineSpeed::unit = "rpm";
}

float EngineSpeed::get_value() {
    return (float) (256*get_a()+get_b())/4;
}