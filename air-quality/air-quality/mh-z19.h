#ifndef __mhz19_h__
#define __mhz19_h__

#include <SoftwareSerial.h>


class MHZ19 {
	public:

	MHZ19(int rx_pin, int tx_pin): serial(rx_pin, tx_pin) {
	}

	void begin();
	int get_co2();
	int get_co2_pwm(int pin);
		

	private:

	SoftwareSerial serial;
};


#endif
