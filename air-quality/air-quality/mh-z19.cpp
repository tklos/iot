#include "mh-z19.h"


#define REQUEST_LEN 9
#define RESPONSE_LEN 9


static byte GET_CO2_CMD[REQUEST_LEN] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};



int MHZ19::get_co2(SoftwareSerial &serial_co2) {
	byte response[RESPONSE_LEN];

	/* Send "Gas Concentration" command */
	serial_co2.write(GET_CO2_CMD, REQUEST_LEN);

	/* Read response */
	while (serial_co2.available() > 0 && (byte)serial_co2.peek() != 0xFF)
		serial_co2.read();

	memset(response, 0, RESPONSE_LEN);
	serial_co2.readBytes(response, RESPONSE_LEN);

	/* Check command */
	if (response[1] != 0x86)
		return -1;

	/* Calculate checksum */
	byte checksum = 0;
	for (int i = 1; i < 8; i++)
		checksum += response[i];
	checksum = 255 - checksum + 1;

	/* Return result */
	if (response[8] != checksum)
		return -1;

	int ppm = (int)response[2] * 256 + (int)response[3];

	return ppm;
}


/**
 * This is not fully correct. One every ~15 requests fails. This is probably
 * caused by calling pulseIn only 10 times inestead of `while (th == 0)`.
 */
int MHZ19::get_co2_pwm(int pin) {
	int MAX_NUM_ITER = 10;

	unsigned long th;
	for (int i = 0; i < MAX_NUM_ITER; i++) {
		th = pulseIn(pin, HIGH, 1004000) / 1000;
		Serial.println(th);
		if (th > 0)
			break;
	}
	if (th <= 0)
		return -1;

	unsigned long tl = 1004 - th;
	int ppm = 5000 * (th - 2) / (th + tl - 4);

	return ppm;
}
