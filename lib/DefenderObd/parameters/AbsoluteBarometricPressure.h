//
// Created by Thomas Spycher on 08.08.22.
//

#ifndef DEFENDEROBD_ABSOLUTEBAROMETRICPRESSURE_H
#define DEFENDEROBD_ABSOLUTEBAROMETRICPRESSURE_H

#include "Parameter.h"

#include "MockSerial_CAN_Module.h"

class AbsoluteBarometricPressure : public Parameter {
public:
#if MOCK_CAN
    AbsoluteBarometricPressure(MockSerial_CAN &can);
#else
    AbsoluteBarometricPressure(Serial_CAN &can);
#endif


    int get_value() override;
};
#endif //DEFENDEROBD_ABSOLUTEBAROMETRICPRESSURE_H
