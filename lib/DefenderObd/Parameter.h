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
    void load_block(unsigned char raw_data[]);
    virtual int get_value() = 0;
    bool request_from_obd(unsigned int timeout_ms=1000);
    int get_previous_value();
    unsigned char get_pid();
    String get_pretty_value();
    String get_name();

    int get_maximum_value();
    int get_minimum_value();

protected:
    Serial_CAN &can;
    int pid;
    String name;
    String unit;

    int previous_value;
    int minimum_value;
    int maximum_value;
    int data_bytes;

    unsigned char *data[8];
    int get_a();
    int get_b();
    int get_c();
    int get_d();
};

#endif //DEFENDEROBD_PARAMETER_H
