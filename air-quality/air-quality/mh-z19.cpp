#include "mh-z19.h"


int MHZ19::get_co2(SoftwareSerial &serial_co2) {
	byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
	byte response[9];

	serial_co2.write(cmd, 9);

	while (serial_co2.available() > 0 && (unsigned char)serial_co2.peek() != 0xFF) {
		serial_co2.read();
	}

	memset(response, 0, 9);
	serial_co2.readBytes(response, 9);

	if (response[1] != 0x86)
		return -1;


	byte crc = 0;
	for (int i = 1; i < 8; i++) {
		crc += response[i];
	}
	crc = 255 - crc + 1;

	if (response[8] == crc) {
		int responseHigh = (int) response[2];
		int responseLow = (int) response[3];
		int ppm = (256 * responseHigh) + responseLow;
		return ppm;
	} else {
		return -1;
	}
}
