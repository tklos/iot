#include <SoftwareSerial.h>
#include <Metro.h>

#include "settings.h"
#include "ds18b20.h"


SoftwareSerial serial_esp8266(serial_esp8266_pins[0], serial_esp8266_pins[1]);

Ds18b20 ds18b20(one_wire_pin);

Metro metro_measurement = Metro(interval_measurement);



void setup() {
	Serial.begin(9600);
	serial_esp8266.begin(9600);
}



void process_single_measurement() {
	/* Print current time */
	Serial.println(String("Millis: ") + millis());


	/* Get temperature */
	float temp = ds18b20.get_temperature();
	if (!ds18b20.is_correct_temperature(temp)) {
		Serial.println("Invalid temperature");
		return;
	}

	String temp_s = String(temp, 2);
	Serial.println(String("Temperature: ") + temp_s);


	/* Send to esp8266 */
	String msg = "<" + temp_s + ">";
	serial_esp8266.print(msg);
}


void loop() {
	/* Get and send temperature */
	if (metro_measurement.check())
		process_single_measurement();


	/* Copy text from esp serial connection */
	while (serial_esp8266.available())
		Serial.write(serial_esp8266.read());
}
