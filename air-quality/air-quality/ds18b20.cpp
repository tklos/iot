#include <Arduino.h>

#include "ds18b20.h"



float DS18B20::get_temperature() {
	int num_reads;
	for (num_reads = 0; num_reads < max_serial_reads; num_reads++) {
		sensors.requestTemperatures();
		float temp = sensors.getTempCByIndex(0);
		if (this->is_correct_temperature(temp))
			return temp;
		else
			delay(delay_between_serial_reads);
	}

	return incorrect_temperature;
}


bool DS18B20::is_correct_temperature(float temp) {
	return temp > incorrect_temperature;
}
