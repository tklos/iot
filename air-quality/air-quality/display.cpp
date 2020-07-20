#include "display.h"


#define BUFLEN 6


static char co2_buf[BUFLEN];
static char o2_buf[BUFLEN];
static char t_buf[BUFLEN];
static char tmp_buf[BUFLEN];


void Display::display_data(String co2, String o2, String t) {
	display.clearDisplay();


	/* Pad strings */
	co2.toCharArray(tmp_buf, BUFLEN);
	snprintf(co2_buf, BUFLEN, "%4s", tmp_buf);

	o2.toCharArray(tmp_buf, BUFLEN);
	snprintf(o2_buf, BUFLEN, "%4s", tmp_buf);

	t.toCharArray(tmp_buf, BUFLEN);
	snprintf(t_buf, BUFLEN, "%5s", tmp_buf);


	/* Print */
	display.setCursor(5, 10);
	display.setTextSize(2);
	display.print(String(co2_buf) + " ppm");

	display.setTextSize(1);
	display.setCursor(105, 17);
	display.print("co2");


	display.setCursor(5, 30);
	display.setTextSize(2);
	display.print(String(o2_buf) + " raw");

	display.setCursor(105, 37);
	display.setTextSize(1);
	display.print("o2");


	display.setCursor(5, 50);
	display.setTextSize(2);
	display.print(String(t_buf) + " \247C");


	display.display();
}
