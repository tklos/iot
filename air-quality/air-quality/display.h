#ifndef __display_h__
#define __display_h__


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



class Display {
	public:

	Display() {
		display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
		if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
			Serial.println("Display initialization failed");

		display.clearDisplay();
		display.setTextColor(WHITE);
	}

	void display_data(String co2, String o2, String t);


	private:

	const int SCREEN_WIDTH = 128;
	const int SCREEN_HEIGHT = 64;


	Adafruit_SSD1306 display;
};


#endif
