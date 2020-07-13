#ifndef __settings_h__
#define __settings_h__


/* Thermometer */
extern const int thermometer_pin;
extern const unsigned long interval_measurement;


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
extern const int max_data_len;


#endif
