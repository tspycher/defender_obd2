//
// Created by Thomas Spycher on 06.08.22.
//

#include "Parameter.h"
#include <Serial_CAN_Module.h>
#include <Arduino.h>


#define STANDARD_CAN_11BIT      1       // That depends on your car. some 1 some 0.


#if STANDARD_CAN_11BIT
#define CAN_ID_PID          0x7DF
#else
#define CAN_ID_PID          0x18db33f1
#endif


Parameter::Parameter(Serial_CAN &can) : can(can), pid(0x00), name("UNKNOWN"), unit("?"), previous_value(0) {}


String Parameter::get_pretty_value() {
    char tmp[16];
    String val = String("RPM is: ") + get_value();
    val.toCharArray(tmp, 16);
    return name + String(" IS: ") + get_value() + unit;
}

int Parameter::get_maximum_value() {
    return maximum_value;
}

int Parameter::get_minimum_value() {
    return minimum_value;
}

bool Parameter::request_from_obd(unsigned int timeout_ms) {

    unsigned long __timeout = millis();
    unsigned char tmp[8] = {0x02, 0x01, get_pid(), 0, 0, 0, 0, 0};
    #if STANDARD_CAN_11BIT
        can.send(CAN_ID_PID, 0, 0, 8, tmp);   // SEND TO ID:0X55
    #else
        can.send(CAN_ID_PID, 1, 0, 8, tmp);   // SEND TO ID:0X55
    #endif

    while(millis()-__timeout < timeout_ms)      // 1s time out
    {
        unsigned long id  = 0;
        unsigned char buf[8];

        if (can.recv(&id, buf)) {
            if(buf[1] == 0x41) {
                load_block(buf);
                return true;
            }
        }
    }
    return false;
}

int Parameter::get_previous_value() {
    return Parameter::previous_value;
}

unsigned char Parameter::get_pid() {
    return pid;
}

String Parameter::get_name() {
    return name;
}

void Parameter::load_block(unsigned char raw_data[]) {
    previous_value = get_value();
    for (int i = 0; i < 8; ++i) {
        data[i] = &raw_data[i];
    }
}

int Parameter::get_a() {
    return (int) *data[3];
}
int Parameter::get_b() {
    return (int) *data[4];
}
int Parameter::get_c() {
    return (int) *data[5];
}
int Parameter::get_d() {
    return (int) *data[6];
}