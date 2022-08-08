//
// Created by Thomas Spycher on 06.08.22.
//


#include <Arduino.h>
#include "DefenderObd.h"

DefenderObd *obd;

bool obd_mock = true;
bool dump_mode = false;


void setup() {
    Serial.begin(115200);
    Serial.println("OBD PoC");

    while(!Serial);

    obd = new DefenderObd();

    /*can.begin(can_tx, can_rx, 9600);      // tx, rx
    Serial.println("begin");
    if (do_debug) {
        Serial.println("RUNNING DEBUG MODE");
    }*/
}

void loop() {
    if (dump_mode) {
        while(true)
            if (!obd->debug())
                break;
    } else {

        int ids[] = {0x0C, 0x51};
        for (int i=0; i<2;++i) {
            Parameter *parameter = obd->get_parameter(ids[i]);
            if (!parameter) {
                continue;
            }
            if (obd_mock) {
                //0x0,0x0,0x0,0x1,0x29,0x0,0x0,0x0
                //0x0,0x0,0x0,0x3,0x8B,0x0,0x0,0x0,
                //0x0	0x0	0x0	0x1	0x29	0x0	0x0	0x0
                //0x0	0x0	0x0	0x3	0x8B	0x0	0x0	0x0
                //0x0	0x0	0x0	0x3	0x8B	0x0	0x0	0x0
                unsigned char fake_data[8] = {0x0,0x0,0x0,0x3,0x8B,0x0,0x0,0x0};

                parameter->load_block(fake_data);
                Serial.print("MOCK DATA: ");
                Serial.println(parameter->get_pretty_value()); //parameter->get_value());
                Serial.println(parameter->get_pid());
            } else {
                if(parameter->request_from_obd()) {
                    Serial.println(parameter->get_pretty_value()); //parameter->get_value());
                } else {
                    Serial.println("Could not load data from OBD");
                }
            }
        }
    }

    delay(1000);
}