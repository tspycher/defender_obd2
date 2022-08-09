//
// Created by Thomas Spycher on 08.08.22.
//

#include "AbsoluteBarometricPressure.h"
#include "DefenderObd.h"

#if MOCK_CAN
AbsoluteBarometricPressure::AbsoluteBarometricPressure(MockSerial_CAN &can) : Parameter(can)  {

}
#else

AbsoluteBarometricPressure::AbsoluteBarometricPressure(Serial_CAN &can) : Parameter(can) {
    AbsoluteBarometricPressure::pid = 0x51;
    AbsoluteBarometricPressure::name = "A.BarPressure";
    AbsoluteBarometricPressure::unit = "kPa";

    pid = 0x33;
    name = "Absolute Barometric Pressure";
    unit = "kPa";
    maximum_value = 255;
    minimum_value = 0;
    data_bytes = 1;
    previous_value = minimum_value;
}
#endif

int AbsoluteBarometricPressure::get_value() {
    return get_a();
}