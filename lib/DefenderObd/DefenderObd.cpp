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
#define can_baud 115200 // 9600


DefenderObd::DefenderObd() {
    parameters[0] = new EngineSpeed(can);
    parameters[1] = new AbsoluteBarometricPressure(can);

    can.begin(can_tx, can_rx, can_baud);
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
        Parameter *p = get_parameter(id);
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