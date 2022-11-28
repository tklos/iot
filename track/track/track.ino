#include <Arduino.h>

#include "settings.h"
#include "gprs.h"
#include "metro.h"


int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;

HardwareSerial &serial = SerialUSB;
HardwareSerial &serial_gprs = Serial1;

GPRS gprs(serial_gprs, baud_gprs);

Metro metro_sms(INTERVAL_SMS);



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
        if (
            ret &&
            gprs.check_ok_anywhere(ret) &&
            (!strncmp(ret, "+CREG: 0,1", 10) || !strncmp(ret, "+CREG: 0,5", 10))
        )
            break;
        serial.print(".");
        delay(500);
    };
    serial.println();


    serial.println("Connected");

    metro_sms.start();
}



void loop() {
	if (metro_sms.check())
		check_sms();

    /* Read from console */
    while (serial.available() > 0) {
        serial_gprs.write(serial.read());
    }
    /* Read from GPRS */
    while (serial_gprs.available() > 0) {
        serial.write(serial_gprs.read());
    }
}



String current_time_millis() {
    unsigned long milli = millis() / 1000;
    int second = milli % 60;
    milli /= 60;
    int minute = milli % 60;
    milli /= 60;
    int hour = milli;

    char buf[15];
    sprintf(buf, "%02d:%02d:%02d", hour, minute, second);
    return buf;
}


void check_sms() {
    int num_smses = gprs.num_smses();
    serial.println(String("Read ") + num_smses + " SMSes");
    if (num_smses <= 0)
        return;

    process_sms();
}


void process_sms() {
    char *msg_s = gprs.read_sms();
    if (!msg_s) {
        serial.println("Can't read SMS");
        return;
    }

    String msg = msg_s;
    msg.toLowerCase();

    if (msg.startsWith("status")) {
        String s = current_time_millis() + "  " + gprs.current_time();
        serial.println(s);

    } else {
        serial.println(String("Can't understand SMS message \"") + msg + "\"");
    }

    gprs.delete_sms();
}
