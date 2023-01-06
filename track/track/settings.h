#ifndef __settings_h__
#define __settings_h__

#include <Arduino.h>



extern const long baud_monitor;


/* GPRS */
extern const long baud_gprs;

extern const unsigned long INTERVAL_SMS;

extern const char *apn;


/* Collect */
extern const char *collect_url;
extern const char *api_key;


/* SD */
extern const String log_dir;


#endif
