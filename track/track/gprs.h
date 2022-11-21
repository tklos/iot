#ifndef __gprs_h__
#define __gprs_h__

#include <Arduino.h>


#define BUFLEN 256
//extern static char *BUF;

#define TIMEOUT_CMD_DEFAULT 5000

#define DELAY_READ_US 90  // Time to send 1 byte is 10/115200 s ~= 87 us


class GPRS {
public:
	GPRS(HardwareSerial &serial, unsigned int baud) : serial(serial) {
		this->baud = baud;
	}

	void begin();

	void send_cmd(const char *cmd);
	char *send_cmd_return(const char *cmd, unsigned int timeout=TIMEOUT_CMD_DEFAULT);
	bool send_cmd_check_ok(const char *cmd, unsigned int timeout=TIMEOUT_CMD_DEFAULT);

private:
	HardwareSerial &serial;
	long baud;

};


#endif
