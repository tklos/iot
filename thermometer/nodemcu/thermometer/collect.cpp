#include <WiFiClientSecure.h>

#include "collect.h"



void Collect::send_data(const String p_data){
	unsigned long start_time = millis();


	/* Connect to server */
	WiFiClientSecure client;
	client.setInsecure();

    if (!client.connect(server, 443)) {
	    Serial.printf("Can't connect to server %s\n", server);
	    client.stop();
	    return;
    }


    /* Prepare request */
	String data = String() + "{\"data\": [" + p_data + "]}";
	String msg =
		String()
		+ "POST " + measurement_resource + " HTTP/1.1\r\n"
		+ "Host: " + host + "\r\n"
		+ "Connection: close\r\n"
		+ "API-KEY: " + api_key + "\r\n"
		+ "Content-Type: application/json\r\n"
		+ "Content-Length: " + data.length() + "\r\n\r\n"
		+ data;


    /* Send request */
    Serial.println(String() + "Sending " + data);
    client.print(msg);


	/* Get response */
	while (!client.available())
		if (millis() - start_time > response_timeout) {
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
