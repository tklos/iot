#ifndef __display_h__
#define __display_h__


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



class Display {
public:
    Display() : display(Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1)) {
    }

    bool begin();

    void print(const String &msg);
    void print(const char *msg);


private:
    Adafruit_SSD1306 display;

    const static int SCREEN_WIDTH = 128;
    const static int SCREEN_HEIGHT = 64;

};


#endif
