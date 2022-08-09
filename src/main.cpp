//
// Created by Thomas Spycher on 06.08.22.
//


#include <Arduino.h>
#include "DefenderObd.h"

DefenderObd *obd;

bool obd_mock = false;
bool dump_mode = false;

bool with_display = true;


void setup() {
    Serial.begin(115200);
    Serial.println("OBD PoC");

    while(!Serial);

    obd = new DefenderObd(with_display);
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
                long r = random(0,3);
                unsigned char x;

                switch(r) {
                    case 0:
                        x = 0x23;
                        break;
                    case 1:
                        x = 0x37;
                        break;
                    case 2:
                        x = 0x5A;
                        break;
                    case 3:
                        x = 0x41;
                        break;
                }
                unsigned char fake_data[8] = {0x0,0x0,0x0,x,0x64,0x0,0x0,0x0};

                parameter->load_block(fake_data);
                Serial.print("MOCK DATA: ");
                Serial.println(parameter->get_pretty_value()); //parameter->get_value());
                obd->update_gauge((int)parameter->get_value() / 10, parameter->get_maximum_value() / 10, "RPM x10");
                delay(500);
            } else {
                if(parameter->request_from_obd()) {
                    Serial.println(parameter->get_pretty_value()); //parameter->get_value());
                    obd->update_gauge((int)parameter->get_value() / 10, parameter->get_maximum_value() / 10, "RPM x10");
                } else {
                    Serial.println("no new OBD Data received");
                    obd->update_gauge((int)parameter->get_previous_value() / 10, parameter->get_maximum_value() / 10, "RPM x10");
                }
            }
        }
    }
}