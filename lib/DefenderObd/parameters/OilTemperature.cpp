//
// Created by Thomas Spycher on 10.08.22.
//

#include "OilTemperature.h"
#include "DefenderObd.h"

#if MOCK_CAN
OilTemperature::OilTemperature(MockSerial_CAN &can) : Parameter(can)  {
    pid = 0x5C;
    name = "Engine oil temperature";
    unit = "C";
    maximum_value = 210;
    minimum_value = -40;
    data_bytes = 1;
    previous_value = minimum_value;
}
#else
OilTemperature::OilTemperature(Serial_CAN &can) : Parameter(can) {
    pid = 0x5C;
    name = "Engine oil temperature";
    unit = "C";
    maximum_value = 210;
    minimum_value = -40;
    data_bytes = 1;
    previous_value = minimum_value;
}
#endif

int OilTemperature::get_value() {
    return get_a() - 40;
}