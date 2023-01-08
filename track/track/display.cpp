#include "display.h"



bool Display::begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
        return false;

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.clearDisplay();
    display.display();

    return true;
}


void Display::print(const String &msg) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(msg);
    display.display();
}


void Display::print(const char *msg) {
    String msg_ = msg;
    print(msg_);
}
