//
// Created by Thomas Spycher on 06.08.22.
//

#ifndef DEFENDEROBD_ENGINESPEED_H
#define DEFENDEROBD_ENGINESPEED_H

#include "Parameter.h"
#include "MockSerial_CAN_Module.h"
#include "DefenderObd.h"

class EngineSpeed : public Parameter {
public:
#if MOCK_CAN
    EngineSpeed(MockSerial_CAN &can);
#else
    EngineSpeed(Serial_CAN &can);
#endif
    int get_value() override;
};


#endif //DEFENDEROBD_ENGINESPEED_H
