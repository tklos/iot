#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include "settings.h"
#include "ds18b20.h"
#include "metro.h"


DS18B20 ds18b20(thermometer_pin);

Metro metro(interval_measurement);



void setup() {
	Serial.begin(115200);


	/* Connect to wifi */
	Serial.printf("Connecting to %s\n", wifi_ssid);
	WiFi.begin(wifi_ssid, wifi_password);

	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println("");
	Serial.println(WiFi.localIP());


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


	/* Send to collect server */
	send_data(temp_s);
}


void send_data(String p_data) {
	unsigned long start_time = millis();


	/* Connect to server */
	WiFiClientSecure client;
	client.setInsecure();

    if (!client.connect(collect_server, 443)) {
	    Serial.printf("Can't connect to server %s\n", collect_server);
	    client.stop();
	    return;
    }


    /* Prepare request */
	String data = String() + "{\"data\": [" + p_data + "]}";
	String msg =
		String()
		+ "POST " + collect_measurement_resource + " HTTP/1.1\r\n"
		+ "Host: " + collect_host + "\r\n"
		+ "Connection: close\r\n"
		+ "API-KEY: " + collect_api_key + "\r\n"
		+ "Content-Type: application/json\r\n"
		+ "Content-Length: " + data.length() + "\r\n\r\n"
		+ data;


    /* Send request */
    Serial.println(String() + "Sending " + data);
    client.print(msg);


	/* Get response */
	while (!client.available())
		if (millis() - start_time > collect_response_timeout) {
			Serial.println("Response timeout");
			return;
		}
	Serial.printf("Client available after %d ms\n", millis() - start_time);


	/* Read headers */
	while (client.connected()) {
		String line = client.readStringUntil('\n');
		if (line == "\r")
			break;
	}


	/* Read response */
	String response = client.readStringUntil('}');
	Serial.println(String() + "Received response: " + response + "} (" + (millis() - start_time) + " ms)");


	/* Read until more data are available */
	String rest = "";
	while (client.available())
		rest += (char)client.read();


	/* Process response */
	Serial.printf("Total time: %d ms\n\n", millis() - start_time);


	client.stop();
}
