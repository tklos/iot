#include "settings.h"
#include "wifi.h"
#include "mh-z19.h"
#include "display.h"
#include "collect.h"
#include "metro.h"


MHZ19 mhz19;

Display display;

Collect collect(
	collect_server,
	collect_port,
	collect_host,
	collect_api_key,
	collect_measurement_resource,
	collect_response_timeout,
	max_data_len
);

SoftwareSerial serial_mhz19(13, 15);

Metro metro(interval_measurement);



void setup() {
	Serial.begin(115200);
	serial_mhz19.begin(9600);

 	connect_to_wifi(wifi_ssid, wifi_password);

	/* Start metro timer */
	metro.reset();
}



void loop() {
	if (metro.check())
		process_single_measurement();
}



void process_single_measurement() {
	/* Print current time */
	Serial.printf("Millis: %lu\n", millis());


	/* Get CO2 measurement */
	int co2 = mhz19.get_co2(serial_mhz19);
	Serial.println(co2);
// 	if (!ds18b20.is_correct_temperature(temp)) {
// 		Serial.println("Invalid temperature");
// 		return;
// 	}
//
// 	String temp_s = String(temp, 2);
// 	Serial.println(String() + "Temperature: " + temp_s);

	String co2_s = String(co2);

	/* Display data */
	display.display_data(co2_s);


	/* Send to collect server */
	String data = String("0,") + co2_s;
	collect.send_data(data);
}
