#include <ESP8266WiFi.h>

#include "wifi.h"


void connect_to_wifi(const char *wifi_ssid, const char *wifi_password) {
	Serial.printf("Connecting to %s\n", wifi_ssid);
	WiFi.begin(wifi_ssid, wifi_password);

	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println("");
	Serial.println(WiFi.localIP());
	Serial.println("");
}
