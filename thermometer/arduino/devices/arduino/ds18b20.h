#ifndef __ds18b20_h__
#define __ds18b20_h__


#include <OneWire.h>
#include <DallasTemperature.h>


class Ds18b20 {
	public:

	Ds18b20(int one_wire_pin, int max_serial_reads=3): one_wire(one_wire_pin), sensors(&one_wire), max_serial_reads(max_serial_reads) {
		sensors.begin();
	}

	float get_temperature();
	bool is_correct_temperature(float temp);


	private:

	OneWire one_wire;
	DallasTemperature sensors;
	int max_serial_reads;

	float incorrect_temperature = -127.;
};


#endif
