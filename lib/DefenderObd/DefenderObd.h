//
// Created by Thomas Spycher on 06.08.22.
//

#ifndef DEFENDEROBD_DEFENDEROBD_H
#define DEFENDEROBD_DEFENDEROBD_H

#define MOCK_CAN      1       // That depends on your car. some 1 some 0.

#include "Parameter.h"

#if MOCK_CAN
#include "MockSerial_CAN_Module.h"
#else
#include <Serial_CAN_Module.h>
#endif
#include <Arduino.h>
#include "Waveshare_LCD1602_RGB.h"

class Parameter;

class DefenderObd {
public:
    DefenderObd(bool with_display=false);
    bool debug();
    int num_parameters();
    Parameter *get_parameter(int pid);
    Parameter *get_parameter(String name);
    void update_gauge(int value, int max_value, String name);
    void show_message(String top, String bottom);
private:
    Parameter *parameters[2];
#if MOCK_CAN
    MockSerial_CAN can;
#else
    Serial_CAN can;
#endif
    Waveshare_LCD1602_RGB *lcd;
    bool with_display;


};


#endif //DEFENDEROBD_DEFENDEROBD_H
