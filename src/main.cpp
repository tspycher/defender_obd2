//
// Created by Thomas Spycher on 06.08.22.
//


#include <Arduino.h>
#include "DefenderObd.h"


DefenderObd *obd;


bool dump_mode = true;
bool with_display = true;

void setup() {
    Serial.begin(115200);
    Serial.println("OBD PoC");

    while (!Serial);

    obd = new DefenderObd(with_display);
}

void loop() {
    if (dump_mode) {
        while (true)
            if (!obd->debug())
                break;
    } else {
        int ids[] = {0x0C , 0x33};
        for (int i = 0; i < 2; ++i) {
            Parameter *parameter = obd->get_parameter(ids[i]);
            if (!parameter) {
                continue;
            }
            if (parameter->request_from_obd()) {
                Serial.println(parameter->get_pretty_value()); //parameter->get_value());
                obd->update_gauge(parameter->get_current_value(), parameter->get_maximum_value(), parameter->get_name());
                delay(500);
            } else {
                Serial.println("no new OBD Data received");
                obd->update_gauge(parameter->get_previous_value(), parameter->get_maximum_value(), parameter->get_name());
            }
        }
    }
}