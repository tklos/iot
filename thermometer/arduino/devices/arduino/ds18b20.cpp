#include "ds18b20.h"


float Ds18b20::get_temperature() {
	int num_reads;
	for (num_reads = 0; num_reads < max_serial_reads; num_reads++) {
		sensors.requestTemperatures();
		float temp = sensors.getTempCByIndex(0);
		if (this->is_correct_temperature(temp))
			return temp;
	}

	return incorrect_temperature;
}


bool Ds18b20::is_correct_temperature(float temp) {
	return temp > incorrect_temperature;
}
