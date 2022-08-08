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


Parameter::Parameter(Serial_CAN &can) : can(can), pid(0x00), name("UNKNOWN"), unit("?") {}


String Parameter::get_pretty_value() {
    char tmp[16];
    String val = String("RPM is: ") + get_value();
    val.toCharArray(tmp, 16);
    return name + String(" IS: ") + get_value() + unit;
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
            Serial.println(id);
            if(buf[1] == 0x41) {
                load_block(buf);
                return true;
            }
        }
    }
    return false;
}

float Parameter::get_previous_value() {
    return Parameter::previous_value;
}

unsigned char Parameter::get_pid() {
    return pid;
}

String Parameter::get_name() {
    return name;
}

void Parameter::load_block(unsigned char raw_data[]) {
    for (int i = 0; i < 8; ++i) {
        data[i] = &raw_data[i];
    }
}

unsigned char Parameter::get_a() {
    return *data[3];
}
unsigned char Parameter::get_b() {
    return *data[4];
}
unsigned char Parameter::get_c() {
    return *data[5];
}
unsigned char Parameter::get_d() {
    return *data[6];
}