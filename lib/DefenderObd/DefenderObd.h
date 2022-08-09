//
// Created by Thomas Spycher on 06.08.22.
//

#ifndef DEFENDEROBD_DEFENDEROBD_H
#define DEFENDEROBD_DEFENDEROBD_H

#include "Parameter.h"
#include <Serial_CAN_Module.h>
#include <Arduino.h>
#include "Waveshare_LCD1602_RGB.h"

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
    Serial_CAN can;
    Waveshare_LCD1602_RGB *lcd;
    bool with_display;


};


#endif //DEFENDEROBD_DEFENDEROBD_H
