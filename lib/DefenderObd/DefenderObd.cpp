//
// Created by Thomas Spycher on 06.08.22.
//

#include "DefenderObd.h"
#include "parameters/EngineSpeed.h"
#include "parameters/AbsoluteBarometricPressure.h"
#include <SoftwareSerial.h>
#include <Arduino.h>

#define can_tx  12       // tx of serial can module, the yellow cable
#define can_rx  13
#define can_baud 9600 // 9600

#define STANDARD_CAN_11BIT      1       // That depends on your car. some 1 some 0.

#if STANDARD_CAN_11BIT
#define CAN_ID_PID          0x7DF
#else
#define CAN_ID_PID          0x18db33f1
#endif

#if STANDARD_CAN_11BIT
unsigned long mask[4] =
{
    0, 0x7FC,                // ext, maks 0
    0, 0x7FC,                // ext, mask 1
};

unsigned long filt[12] =
{
    0, 0x7E8,                // ext, filt 0
    0, 0x7E8,                // ext, filt 1
    0, 0x7E8,                // ext, filt 2
    0, 0x7E8,                // ext, filt 3
    0, 0x7E8,                // ext, filt 4
    0, 0x7E8,                // ext, filt 5
};

#else
unsigned long mask[4] =
        {
                1, 0x1fffffff,               // ext, maks 0
                1, 0x1fffffff,
        };

unsigned long filt[12] =
        {
                1, 0x18DAF110,                // ext, filt
                1, 0x18DAF110,                // ext, filt 1
                1, 0x18DAF110,                // ext, filt 2
                1, 0x18DAF110,                // ext, filt 3
                1, 0x18DAF110,                // ext, filt 4
                1, 0x18DAF110,                // ext, filt 5
        };
#endif



DefenderObd::DefenderObd() {
    parameters[0] = new EngineSpeed(can);
    parameters[1] = new AbsoluteBarometricPressure(can);

    can.begin(can_tx, can_rx, can_baud);
    can.setMask(mask);
    can.setFilt(filt);
}

int DefenderObd::num_parameters() {
    return (int) sizeof(*parameters)/sizeof(parameters[0]);
}

Parameter *DefenderObd::get_parameter(String name) {
    for (int i=0; i<num_parameters(); ++i) {
        if (parameters[i]->get_name() == name)
            return parameters[i];
    }
    return NULL;
}

Parameter *DefenderObd::get_parameter(int pid) {
    for (int i=0; i<num_parameters(); ++i) {
        if (parameters[i]->get_pid() == pid)
            return parameters[i];
    }
    return NULL;
}

bool DefenderObd::debug() {
    unsigned long id = 0;
    unsigned char dta[8];

    if(can.recv(&id, dta))
    {
        if (id < 2)
            return false;

        Serial.print("GET DATA FROM ID: ");
        Serial.print(id);
        Serial.print(" Data -> ");
        for(int i=0; i<8; i++)
        {
            Serial.print("0x");
            Serial.print(dta[i], HEX);
            Serial.print(',');
        }
        Serial.println();
        Parameter *p = get_parameter(dta[2]); // id
        if (p) {
            p->load_block(dta);
            Serial.print("\tFOUND PARAMETER! Value: ");
            Serial.println(p->get_value());
        }
        return true;
    }
    Serial.println("no data received");
    return false;
}