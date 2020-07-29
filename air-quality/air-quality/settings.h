#ifndef __settings_h__
#define __settings_h__


/* Measurement */
extern const unsigned long interval_measurement;


/* Devices */
extern const int mhz19_rx_pin;
extern const int mhz19_tx_pin;
extern const int meo2d0_pin;


/* WiFi */
extern const char *wifi_ssid;
extern const char *wifi_password;


/* Collect server */
extern const char *collect_server;
extern const int collect_port;
extern const char *collect_host;
extern const char *collect_api_key;
extern const char *collect_measurement_resource;
extern const unsigned long collect_response_timeout;


#endif
