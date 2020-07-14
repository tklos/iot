#include "settings.h"
#include "wifi.h"
#include "mh-z19.h"
#include "me-o2-d20.h"
#include "ds18b20.h"
#include "display.h"
#include "collect.h"
#include "metro.h"


#define MHZ19_RX_PIN 13
#define MHZ19_TX_PIN 15
#define MEO2D20_PIN A0
#define DS18B20_PIN 12



MHZ19 mhz19;
MEO2D20 device_o2(MEO2D20_PIN);
DS18B20 thermometer(DS18B20_PIN);

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

SoftwareSerial serial_mhz19(MHZ19_RX_PIN, MHZ19_TX_PIN);

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

	String co2_s = (co2 == -1) ? String("null") : String(co2);


	/* Get O2 measurement */
	struct o2_measurement o2;
	device_o2.get_o2(o2);
	Serial.println(o2.raw);

	String o2_s = (o2.raw < 10 || o2.raw >= 600) ? String("null") : String(o2.raw);


	/* Get temperature measurement */
	float temperature = thermometer.get_temperature();
	String temperature_s = (thermometer.is_correct_temperature(temperature)) ? String(temperature, 2) : String("null");
	Serial.println(temperature_s);


	/* Display data */
	display.display_data(co2_s, o2_s, temperature_s);


	/* Send to collect server */
	String data = co2_s + "," + o2_s + "," + temperature_s;
	collect.send_data(data);
}
