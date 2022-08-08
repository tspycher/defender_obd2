//
// Created by Thomas Spycher on 06.08.22.
//

#ifndef DEFENDEROBD_ENGINESPEED_H
#define DEFENDEROBD_ENGINESPEED_H

#include "Parameter.h"

class EngineSpeed : public Parameter {
public:
    EngineSpeed(Serial_CAN &can);
    float get_value() override;
};


#endif //DEFENDEROBD_ENGINESPEED_H
