#include <BME280I2C.h>

#include "settings.h"
#include "wifi.h"
#include "mh-z19.h"
#include "me-o2-d20.h"
#include "display.h"
#include "collect.h"
#include "metro.h"



MHZ19 device_co2(mhz19_rx_pin, mhz19_tx_pin);
MEO2D20 device_o2(meo2d0_pin);
BME280I2C bme280;

Display display;

Collect collect(
	collect_server,
	collect_port,
	collect_host,
	collect_api_key,
	collect_measurement_resource,
	collect_response_timeout
);

Metro metro(interval_measurement);



void setup() {
	Serial.begin(115200);

	device_co2.begin();
	bme280.begin();

 	connect_to_wifi(wifi_ssid, wifi_password);

	metro.start();
}



void loop() {
	if (metro.check())
		process_single_measurement();
}



void process_single_measurement() {
	/* Print current time */
	Serial.printf("Millis: %lu\n", millis());


	/* CO2 */
	int co2 = device_co2.get_co2();
	Serial.println(co2);

	String co2_s = (co2 == -1) ? String("null") : String(co2);


	/* O2 */
	struct o2_measurement o2;
	device_o2.get_o2(o2);
	Serial.println(o2.raw);

	String o2_s = (o2.raw < 10 || o2.raw >= 600) ? String("null") : String(o2.raw);


	/* BME280 */
	float t, h, p;
	bme280.read(p, t, h, BME280::TempUnit_Celsius, BME280::PresUnit_hPa);

	String t_s = isnan(t) ? String("null") : String(t, 1);
	String h_s = isnan(h) ? String("null") : String(h, 1);
	String p_s = isnan(p) ? String("null") : String(p, 1);


	/* Display data */
	display.display_data(co2_s, o2_s, t_s, h_s, p_s);


	/* Send to collect server */
	String data = co2_s + "," + o2_s + "," + t_s + "," + h_s + "," + p_s;
	collect.send_data(data);
}
