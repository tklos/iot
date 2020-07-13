#include <Arduino.h>

#include "me-o2-d20.h"


int MEO2D20::get_o2(struct o2_measurement &o2) {
	/* Get raw reading */
	int raw = analogRead(pin);

	/* TODO Get calibrated value */
	float value = -1;

	/* Save data */
	o2.raw = raw;
	o2.value = value;

	return 0;
}
