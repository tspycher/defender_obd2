//
// Created by Thomas Spycher on 06.08.22.
//

#ifndef DEFENDEROBD_PARAMETER_H
#define DEFENDEROBD_PARAMETER_H

#include <Serial_CAN_Module.h>
#include <Arduino.h>

class Parameter {
public:
    Parameter(Serial_CAN &can);
    void load_block(unsigned char raw_data[8]);
    virtual float get_value() = 0;
    bool request_from_obd(unsigned int timeout_ms=1000);
    float get_previous_value();
    int get_pid();
    String get_pretty_value();
    String get_name();
protected:
    Serial_CAN &can;
    int pid;
    String name;
    String unit;

    float previous_value;
    unsigned char *data[8];
    unsigned char get_a();
    unsigned char get_b();
    unsigned char get_c();
    unsigned char get_d();
};

#endif //DEFENDEROBD_PARAMETER_H
