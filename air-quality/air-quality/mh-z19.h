#ifndef __mhz19_h__
#define __mhz19_h__

#include <SoftwareSerial.h>


class MHZ19 {
	public:

	int get_co2(SoftwareSerial &serial_co2);
	int get_co2_pwm(int pin);
		

	private:
};


#endif
