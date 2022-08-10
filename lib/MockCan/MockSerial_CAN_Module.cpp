//
// Created by Thomas Spycher on 09.08.22.
//

#include "MockSerial_CAN_Module.h"
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <Arduino.h>


void MockSerial_CAN::begin(int can_tx, int can_rx, unsigned long baud) {
    /*softwareSerial = new SoftwareSerial(can_tx, can_rx);
    softwareSerial->begin(baud);
    canSerial = softwareSerial;
     */
    Serial.println("************** MOCK CAN STARTING UP **************");
}

void MockSerial_CAN::begin(SoftwareSerial &serial, unsigned long baud) {
    /*serial.begin(baud);
    softwareSerial = &serial;
    canSerial = &serial;
    */
}

void MockSerial_CAN::begin(HardwareSerial &serial, unsigned long baud) {
    /*serial.begin(baud);
    hardwareSerial = &serial;
    canSerial = &serial;
     */
}

unsigned char MockSerial_CAN::send(unsigned long id, uchar ext, uchar rtrBit, uchar len, const uchar *buf) {
    unsigned char dta[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    dta[0] = id >> 24;        // id3
    dta[1] = id >> 16 & 0xff;   // id2
    dta[2] = id >> 8 & 0xff;    // id1
    dta[3] = id & 0xff;       // id0

    dta[4] = ext;
    dta[5] = rtrBit;

    for (int i = 0; i < len; i++) {
        dta[6 + i] = buf[i];
    }

    Serial.print("Writing Data to canSerial: ");
    for (int i = 0; i < 14; i++) {
        Serial.print(dta[i]);
        Serial.print(" ");
    }
    Serial.println();
}


// 0: no data
// 1: get data
unsigned char MockSerial_CAN::recv(unsigned long *id, uchar *buf) {
    int len = 0;
    uchar dta[20];

    uchar a;
    uchar b;

    switch (millis() % 4) {
        case 0:
            *id = 0x0C; // RPM PID
            a = 0x00 + random(20,120);
            b = 0x64;
            break;
        case 1:
            *id = 0x0D; // Car Speed
            a = 0x53;
            b = 0x00;
            break;
        case 2:
            *id = 0x5C; // Oil Temperature
            a = 0x8c;
            b = 0x00;
            break;
        case 3:
            *id = 0x33; // Absolut Barometric Pressure
            a = 0x80;
            b = 0x00;
            break;
    }

    // meta
    dta[0] = 0x00;
    ++len;
    dta[1] = 0x00;
    ++len;
    dta[2] = 0x07;
    ++len;
    dta[3] = 0x00;
    ++len;

    // Response
    dta[4] = 0x00;
    ++len;
    dta[5] = 0x41;
    ++len;
    dta[6] = *id;
    ++len;

    // A
    dta[7] = a;
    ++len;

    // B
    dta[8] = b;
    ++len;

    // C
    dta[9] = 0x00;
    ++len;

    // D
    dta[10] = 0x00;
    ++len;

    // ???
    dta[11] = 0x00;
    ++len;

    for (int i = 0; i < 8; i++) // Store the message in the buffer
    {
        buf[i] = dta[i + 4];
    }
    return 1;

}

unsigned char MockSerial_CAN::cmdOk(char *cmd) {

    unsigned long timer_s = millis();
    unsigned char len = 0;

    //canSerial->println(cmd);
    Serial.print("Sending command: ");
    Serial.println(cmd);
    while (1) {
        if (millis() - timer_s > 500) {
            return 0;
        }

        while (true) {
            str_tmp[len++] = 0x00; //canSerial->read();
            timer_s = millis();
            if (len >= 12)
                break;
        }

        if (len >= 4 && str_tmp[len - 1] == '\n' && str_tmp[len - 2] == '\r' && str_tmp[len - 3] == 'K' &&
            str_tmp[len - 4] == 'O') {
            clear();
            return 1;
        }
    }
}

/*
value	    01	02	03	04	05	    06	07	08	09	10	    11	12	13	14	15	16	17	18
rate(kb/s)	5	10	20	25	31.2	33	40	50	80	83.3	95	100	125	200	250	500	666	1000
*/
unsigned char MockSerial_CAN::canRate(unsigned char rate) {
    enterSettingMode();
    if (rate < 10)
        sprintf(str_tmp, "AT+C=0%d\r\n", rate);
    else
        sprintf(str_tmp, "AT+C=%d\r\n", rate);

    int ret = cmdOk(str_tmp);

    exitSettingMode();
    return ret;
}

/*
value	        0	    1	    2	    3   	4
baud rate(b/s)	9600	19200	38400	/	115200
*/

unsigned char MockSerial_CAN::baudRate(unsigned char rate) {
    unsigned long baud[5] = {9600, 19200, 38400, 9600, 115200};
    int baudNow = 0;

    if (rate == 3)return 0;

    for (int i = 0; i < 5; i++) {
        selfBaudRate(baud[i]);
        Serial.print("+++");
        //canSerial->print("+++");
        delay(100);

        if (cmdOk("AT\r\n")) {
            Serial.print("SERIAL BAUD RATE IS: ");
            Serial.println(baud[i]);
            baudNow = i;
            break;
        }
    }

    sprintf(str_tmp, "AT+S=%d\r\n", rate);
    cmdOk(str_tmp);

    selfBaudRate(baud[rate]);

    int ret = cmdOk("AT\r\n");

    if (ret) {
        Serial.print("Serial baudrate set to ");
        Serial.println(baud[rate]);
    }

    exitSettingMode();
    return ret;
}

void MockSerial_CAN::selfBaudRate(unsigned long baud) {
    /*if(softwareSerial)
    {
        softwareSerial->begin(baud);
    }
    else if(hardwareSerial)
    {
        hardwareSerial->begin(baud);
    }*/
}

void MockSerial_CAN::clear() {
    unsigned long timer_s = millis();
    while (1) {
        if (millis() - timer_s > 50)return;
        while (true) {
            //canSerial->read();
            timer_s = millis();
            break;
        }
    }
}

unsigned char MockSerial_CAN::enterSettingMode() {
    Serial.print("+++");
    //canSerial->print("+++");
    clear();
    return 1;
}

unsigned char MockSerial_CAN::exitSettingMode() {
    clear();
    int ret = cmdOk((char *) "AT+Q\r\n");
    clear();
    return ret;
}

void make8zerochar(int n, char *str, unsigned long num) {
    for (int i = 0; i < n; i++) {
        str[n - 1 - i] = num % 0x10;
        if (str[n - 1 - i] < 10)str[n - 1 - i] += '0';
        else str[n - 1 - i] = str[n - 1 - i] - 10 + 'A';
        num >>= 4;
    }
    str[n] = '\0';
}

/*
+++	                    Switch from Normal mode to Config mode
AT+S=[value]	        Set serial baud rate
AT+C=[value]	        Set CAN Bus baud rate
AT+M=[N][EXT][value]    Set mask,AT+M=[1][0][000003DF]
AT+F=[N][EXT][value]    Set filter,AT+F=[1][0][000003DF]
AT+Q	            Switch to Normal Mode
*/
unsigned char MockSerial_CAN::setMask(unsigned long *dta) {
    enterSettingMode();
    char __str[10];


    for (int i = 0; i < 2; i++) {
        make8zerochar(8, __str, dta[1 + 2 * i]);
        //Serial.println(__str);
        sprintf(str_tmp, "AT+M=[%d][%d][", i, dta[2 * i]);
        for (int i = 0; i < 8; i++) {
            str_tmp[12 + i] = __str[i];
        }
        str_tmp[20] = ']';
        str_tmp[21] = '\r';
        str_tmp[22] = '\n';
        str_tmp[23] = '\0';

        //Serial.println(str_tmp);

        if (!cmdOk(str_tmp)) {
            Serial.print("mask fail - ");
            Serial.println(i);
            exitSettingMode();
            return 0;
        }
        clear();
        delay(10);
        //
    }
    exitSettingMode();
    return 1;

}

unsigned char MockSerial_CAN::setFilt(unsigned long *dta) {
    enterSettingMode();

    char __str[10];

    for (int i = 0; i < 6; i++) {
        make8zerochar(8, __str, dta[1 + 2 * i]);
        //Serial.println(__str);
        sprintf(str_tmp, "AT+F=[%d][%d][", i, dta[2 * i]);
        for (int i = 0; i < 8; i++) {
            str_tmp[12 + i] = __str[i];
        }
        str_tmp[20] = ']';
        str_tmp[21] = '\r';
        str_tmp[22] = '\n';
        str_tmp[23] = '\0';

        //Serial.println(str_tmp);

        clear();
        if (!cmdOk(str_tmp)) {
            //Serial.print("filt fail at - ");
            //Serial.println(i);
            exitSettingMode();
            return 0;
        }
        clear();
        delay(10);
        //
    }
    exitSettingMode();
    return 1;
}

/*
value	        0	    1	    2	    3   	4
baud rate(b/s)	9600	19200	38400	57600	115200
*/
unsigned char MockSerial_CAN::factorySetting() {
    // check baudrate
    unsigned long baud[5] = {9600, 19200, 38400, 57600, 115200};

    for (int i = 0; i < 5; i++) {
        selfBaudRate(baud[i]);
        Serial.print("+++");
        //canSerial->print("+++");
        delay(100);

        if (cmdOk("AT\r\n")) {
            Serial.print("SERIAL BAUD RATE IS: ");
            Serial.println(baud[i]);
            baudRate(0);                // set serial baudrate to 9600
            Serial.println("SET SERIAL BAUD RATE TO: 9600 OK");
            selfBaudRate(9600);
            break;
        }
    }

    if (canRate(CAN_RATE_500)) {
        Serial.println("SET CAN BUS BAUD RATE TO 500Kb/s OK");
    } else {
        Serial.println("SET CAN BUS BAUD RATE TO 500Kb/s FAIL");
        return 0;
    }

    unsigned long mask[4] = {0, 0, 0, 0,};
    unsigned long filt[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};

    if (setFilt(filt)) {
        Serial.println("FACTORY SETTING FILTS OK");
    } else {
        Serial.println("FACTORY SETTING FILTS FAIL");
        return 0;
    }

    if (setMask(mask)) {
        Serial.println("FACTORY SETTING MASKS OK");
    } else {
        Serial.println("FACTORY SETTING MASKS FAIL");
        return 0;
    }

    return 1;
}

void MockSerial_CAN::debugMode() {
    while (Serial.available()) {
        //canSerial->write(Serial.read());
        //Serial.println(Serial.read());
        Serial.println("What now?");
    }

    while (true) {
        Serial.write("ehmm?");
    }
}
