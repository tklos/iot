#include <Arduino.h>

#include "settings.h"
#include "gprs.h"


int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;

HardwareSerial &serial = SerialUSB;
HardwareSerial &serial_gprs = Serial1;

GPRS gprs(serial_gprs, baud_gprs);



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
	serial.print("Switch off echo ");
	while (1) {
		bool ret = gprs.send_cmd_check_ok("ATE0");
		if (ret)
			break;
		serial.print(".");
		delay(500);
	};
	serial.println();


	serial.print("AT ");
	while (1) {
		bool ret = gprs.send_cmd_check_ok("AT");
		if (ret)
			break;
		serial.print(".");
		delay(500);
	};
	serial.println();


	serial.print("Disable unsolicited network registration messages ");
	while (1) {
		bool ret = gprs.send_cmd_check_ok("AT+CREG=0");
		if (ret)
			break;
		serial.print(".");
		delay(500);
	};
	serial.println();


	serial.print("Network registration ");
	while (1) {
		char *ret = gprs.send_cmd_return("AT+CREG?");
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
	while (Serial1.available() > 0) {
		SerialUSB.write(Serial1.read());
	}
}
