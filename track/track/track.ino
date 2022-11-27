#include <Arduino.h>

#include "settings.h"
#include "gprs.h"


int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;

HardwareSerial &serial = SerialUSB;
HardwareSerial &serial_gprs = Serial1;

GPRS gprs(serial_gprs, baud_gprs);



void setuperror() {
    serial.println("Error");
    while (true) {
    }
}


void setup() {
    /* Device setup */
    pinMode(PWR_KEY, OUTPUT);
    pinMode(RST_KEY, OUTPUT);
    pinMode(LOW_PWR_KEY, OUTPUT);
    digitalWrite(RST_KEY, LOW);
    digitalWrite(LOW_PWR_KEY, HIGH);
    digitalWrite(PWR_KEY, HIGH);
    digitalWrite(PWR_KEY, LOW);
    delay(3000);
    digitalWrite(PWR_KEY, HIGH);
    delay(3000);

    serial.begin(baud_monitor);
    gprs.begin();

    serial.println("Ready");


    /* GPRS */
    serial.println("Switch off echo");
    bool ret = gprs.send_cmd_check_ok_anywhere("ATE0", 40);
    if (!ret)
        setuperror();

    serial.println("AT");
    ret = gprs.send_cmd_check_ok("AT");
    if (!ret)
        setuperror();

    serial.println("Disable unsolicited network registration messages");
    ret = gprs.send_cmd_check_ok("AT+CREG=0");
    if (!ret)
        setuperror();

    serial.println("Switch off new SMS indications");
    ret = gprs.send_cmd_check_ok("AT+CNMI=0,0,0,0,0");
    if (!ret)
        setuperror();

    serial.println("Switch off indications");
    ret = gprs.send_cmd_check_ok("AT+CMER=3,0,0,0");
    if (!ret)
        setuperror();

    serial.println("Set SMS format to text");
    ret = gprs.send_cmd_check_ok("AT+CMGF=1");
    if (!ret)
        setuperror();

    serial.println("Set SMS storage");
    ret = gprs.send_cmd_check_ok_anywhere("AT+CPMS=\"SM\",\"SM\",\"SM\"");
    if (!ret)
        setuperror();

    serial.print("Wait for network registration ");
    while (true) {
        char *ret = gprs.send_cmd_return("AT+CREG?");
        if (!ret)
            continue;
        if (!strncmp(ret, "+CREG: 0,1", 10) || !strncmp(ret, "+CREG: 0,5", 10))
            break;
        serial.print(".");
        delay(500);
    };
    serial.println();


    serial.println("Connected");
}



void loop() {
    /* Read from console */
    while (serial.available() > 0) {
        serial_gprs.write(serial.read());
    }
    /* Read from GPRS */
    while (serial_gprs.available() > 0) {
        serial.write(serial_gprs.read());
    }
}
