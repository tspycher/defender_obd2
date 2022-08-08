//
// Created by Thomas Spycher on 08.08.22.
//

#ifndef DEFENDEROBD_ABSOLUTEBAROMETRICPRESSURE_H
#define DEFENDEROBD_ABSOLUTEBAROMETRICPRESSURE_H

#include "Parameter.h"


class AbsoluteBarometricPressure : public Parameter {
public:
    AbsoluteBarometricPressure(Serial_CAN &can);
    float get_value() override;
};
#endif //DEFENDEROBD_ABSOLUTEBAROMETRICPRESSURE_H
