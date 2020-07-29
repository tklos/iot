#include "display.h"


#define BUFLEN 7


static char co2_buf[BUFLEN], o2_buf[BUFLEN], t_buf[BUFLEN], h_buf[BUFLEN], p_buf[BUFLEN];
static char tmp_buf[BUFLEN];


void Display::display_data(String co2, String o2, String t, String h, String p) {
	display.clearDisplay();


	/* Pad strings */
	co2.toCharArray(tmp_buf, BUFLEN);
	snprintf(co2_buf, BUFLEN, "%4s", tmp_buf);

	o2.toCharArray(tmp_buf, BUFLEN);
	snprintf(o2_buf, BUFLEN, "%4s", tmp_buf);

	t.toCharArray(tmp_buf, BUFLEN);
	snprintf(t_buf, BUFLEN, "%4s", tmp_buf);

	h.toCharArray(tmp_buf, BUFLEN);
	snprintf(h_buf, BUFLEN, "%5s", tmp_buf);

	p.toCharArray(tmp_buf, BUFLEN);
	snprintf(p_buf, BUFLEN, "%6s", tmp_buf);


	/* Print */
	/* CO2 */
	display.setCursor(0, 0);
	display.setTextSize(2);
	display.print(String(co2_buf) + " ppm");

	display.setTextSize(1);
	display.setCursor(100, 7);
	display.print("co2");


	/* O2 */
	display.setCursor(0, 17);
	display.setTextSize(2);
	display.print(String(o2_buf) + " raw");

	display.setCursor(100, 24);
	display.setTextSize(1);
	display.print("o2");


	/* Temperature */
	display.setCursor(0, 34);
	display.setTextSize(2);
	display.print(String(t_buf) + " \247C");


	/* Humidity */
	display.setCursor(0, 56);
	display.setTextSize(1);
	display.print(String(h_buf) + " %");


	/* Pressure */
	display.setCursor(64, 56);
	display.setTextSize(1);
	display.print(String(p_buf) + " hPa");


	display.display();
}
