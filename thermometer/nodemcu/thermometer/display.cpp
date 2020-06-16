#include "display.h"


void Display::display_temperature(String temp) {
	display.clearDisplay();

	display.setTextSize(2);
	display.setCursor(5, 20);
	display.println(temp + " \247C");

	display.display();
}
