//
// Created by Thomas Spycher on 10.08.22.
//

#ifndef DEFENDEROBD_OILTEMPERATURE_H
#define DEFENDEROBD_OILTEMPERATURE_H
#include "Parameter.h"

#include "MockSerial_CAN_Module.h"

class OilTemperature : public Parameter {
public:
#if MOCK_CAN
    OilTemperature(MockSerial_CAN &can);
#else
    OilTemperature(Serial_CAN &can);
#endif
    int get_value() override;
};

#endif //DEFENDEROBD_OILTEMPERATURE_H
