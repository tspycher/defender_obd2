//
// Created by Thomas Spycher on 06.08.22.
//

#ifndef DEFENDEROBD_PARAMETER_H
#define DEFENDEROBD_PARAMETER_H

#include <Serial_CAN_Module.h>
#include <MockSerial_CAN_Module.h>
#include <Arduino.h>
#include <DefenderObd.h>

class Parameter {
public:
#if MOCK_CAN
    Parameter(MockSerial_CAN &can);
#else
    Parameter(Serial_CAN &can);
#endif
    void load_block(unsigned char raw_data[]);
    bool request_from_obd(unsigned int timeout_ms=1000);
    int get_current_value();
    int get_previous_value();
    unsigned char get_pid();
    String get_pretty_value();
    String get_name();

    int get_maximum_value();
    int get_minimum_value();

protected:
    virtual int get_value() = 0;

#if MOCK_CAN
    MockSerial_CAN &can;
#else
    Serial_CAN &can;
#endif
    int pid;
    String name;
    String unit;

    int current_value;
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
