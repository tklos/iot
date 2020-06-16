#include "settings.h"
#include "wifi.h"
#include "ds18b20.h"
#include "display.h"
#include "collect.h"
#include "metro.h"


DS18B20 ds18b20(thermometer_pin);
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

Metro metro(interval_measurement);



void setup() {
	Serial.begin(115200);

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


	/* Get temperature */
	float temp = ds18b20.get_temperature();
	if (!ds18b20.is_correct_temperature(temp)) {
		Serial.println("Invalid temperature");
		return;
	}

	String temp_s = String(temp, 2);
	Serial.println(String() + "Temperature: " + temp_s);


	/* Display temperature */
	display.display_temperature(temp_s);


	/* Send to collect server */
	collect.send_data(temp_s);
}
