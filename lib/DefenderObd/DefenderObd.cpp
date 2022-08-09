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



DefenderObd::DefenderObd(bool with_display) : with_display(with_display) {
    parameters[0] = new EngineSpeed(can);
    parameters[1] = new AbsoluteBarometricPressure(can);

    can.begin(can_tx, can_rx, can_baud);
    can.setMask(mask);
    can.setFilt(filt);

    if (with_display) {
        lcd = new Waveshare_LCD1602_RGB(16, 2);  //16 characters and 2 lines of show
        lcd->init();
        lcd->noCursor();
        lcd->setRGB(255, 0, 255);
        lcd->setCursor(0, 0);

        lcd->send_string("K.I.T.T.T.T.");

        lcd->setCursor(0, 1);
        lcd->send_string("Fucking Can-Bus");
        delay(2000);
    }
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

        if (with_display) {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->write_char(id);

            lcd->setCursor(0, 1);
        }
        Serial.print("GET DATA FROM ID: ");
        Serial.print(id);
        Serial.print(" Data -> ");
        for(int i=0; i<8; i++)
        {
            Serial.print("0x");
            Serial.print(dta[i], HEX);
            Serial.print(',');
            if (with_display)
                lcd->write_char(dta[i]);
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
    show_message("* OBD DEBUG ****", "NO DATA!");
    return false;
}

void DefenderObd::show_message(String top, String bottom) {
    if (!with_display)
        return;
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->send_string(top.c_str());
    lcd->setCursor(0, 1);
    lcd->send_string(bottom.c_str());
}

void DefenderObd::update_gauge(int value, int max_value, String name) {
    if (!with_display)
        return;

    unsigned int lcd_cols = 16;
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->send_string(name.c_str());

    unsigned int number_of_digits = 0;
    int n = value;
    do {
        ++number_of_digits;
        n /= 10;
    } while (n);

    lcd->setCursor(0, 1);
    unsigned int value_percentage = 100 / max_value * value;
    unsigned int lcd_gauge_value = (int) (lcd_cols/ 100.0 * (float)value_percentage);

    char filler = 255;
    char empty = 219;

    String gauge;

    for (unsigned int i = 0; i < lcd_cols - number_of_digits; i++) {
        if(i<= lcd_gauge_value) {
            lcd->write_char(filler); //(char)32);
        } else {
            lcd->write_char(empty); //(char)32);
        }
    }
    lcd->send_string(String(value).c_str());
}