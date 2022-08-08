//
// Created by Thomas Spycher on 06.08.22.
//

#ifndef DEFENDEROBD_DEFENDEROBD_H
#define DEFENDEROBD_DEFENDEROBD_H

#include "Parameter.h"
#include <Serial_CAN_Module.h>
#include <Arduino.h>

class DefenderObd {
public:
    DefenderObd();
    bool debug();
    int num_parameters();
    Parameter *get_parameter(int pid);
    Parameter *get_parameter(String name);
private:
    Parameter *parameters[2];
    Serial_CAN can;

};


#endif //DEFENDEROBD_DEFENDEROBD_H
