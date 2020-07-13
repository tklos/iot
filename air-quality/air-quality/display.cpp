#include "display.h"


void Display::display_data(String co2, String co2_pwm) {
	display.clearDisplay();

	display.setTextSize(2);

	display.setCursor(5, 20);
	display.println(co2 + " ppm");

	display.setCursor(5, 50);
	display.println(co2_pwm + " ppm");

	display.display();
}
