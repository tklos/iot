#include "display.h"


void Display::display_data(String co2) {
	display.clearDisplay();

	display.setTextSize(2);

	display.setCursor(5, 20);
	display.println(co2 + " ppm");

	display.display();
}
