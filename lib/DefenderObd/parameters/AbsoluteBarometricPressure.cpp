//
// Created by Thomas Spycher on 08.08.22.
//

#include "AbsoluteBarometricPressure.h"


AbsoluteBarometricPressure::AbsoluteBarometricPressure(Serial_CAN &can) : Parameter(can) {
    AbsoluteBarometricPressure::pid = 0x51;
    AbsoluteBarometricPressure::name = "A.BarPressure";
    AbsoluteBarometricPressure::unit = "kPa";
}

float AbsoluteBarometricPressure::get_value() {
    return (float) (256*get_a()+get_b())/4;
}