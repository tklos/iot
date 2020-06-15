#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include "settings.h"


bool receiving_data = false;
String arduino_data;



void setup() {
	Serial.begin(9600);


	/* Connect to wifi */
	Serial.printf("Connecting to %s\n", wifi_ssid);
	WiFi.begin(wifi_ssid, wifi_password);

	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}

	Serial.println(WiFi.localIP());
}



void send_data(String p_data) {
	unsigned long function_start_time = millis();


	/* Connect to server */
	WiFiClientSecure client;
	client.setInsecure();

	if (!client.connect(collect_server, 443)) {
		Serial.println(String("Can't connect to server") + collect_server);
		client.stop();
		return;
	}


	/* Send request */
	String data = String("{\"data\": [") + p_data + "]}";
	String msg = String("POST ") + collect_measurement_resource + " HTTP/1.1\r\n"
			+ "Host: " + collect_host + "\r\n"
			+ "API-KEY: " + collect_api_key + "\r\n"
			+ "Content-Type: application/json\r\n"
			+ "Content-Length: " + data.length() + "\r\n\r\n"
			+ data;

	Serial.println(String("Sending ") + data);
	client.print(msg);


	/* Get response */
	unsigned long start_time = millis();
	while (!client.available())
		if (millis() - start_time > collect_response_timeout) {
			Serial.println("Response timeout");
			return;
		}
	Serial.printf("Client available after %d ms\n", millis() - start_time);

	// Read headers
	while (client.connected()) {
		String line = client.readStringUntil('\n');
		if (line == "\r")
			break;
	}

	// TODO: Read until end of message
	String response = client.readStringUntil('}');
	Serial.println(String("Received response: ") + response + "} (" + (millis() - start_time) + " ms)");


	/* Process response */
	Serial.printf("Total time: %d ms\n\n", millis() - function_start_time);


	client.stop();
}


void loop() {
	/* Read data from Arduino */
	while (Serial.available()) {
		char c = Serial.read();
		switch (c) {
			case '<':
				receiving_data = true;
				arduino_data = "";
				break;

			case '>': {
				// Send measurement
				int data_len = arduino_data.length();
				if (data_len > 0 && data_len <= max_data_len)
					send_data(arduino_data);
				else
					Serial.println(String("Error: Not sending \"") + arduino_data + "\"");

				// Reset buffer
				receiving_data = false;
				arduino_data = "";
				break;
			}

			default:
				if (receiving_data)
					arduino_data += c;
				else
					Serial.printf("Error: Unknown character \"%c\" (%d)\n", c, c);
				break;
		}
	}
}
