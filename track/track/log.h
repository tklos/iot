#ifndef __log_h__
#define __log_h__


#include <Arduino.h>

#include "gprs.h"
#include "display.h"
#include "sd.h"


#define LOG_SERIAL 1
#define LOG_DISPLAY 2
#define LOG_SD 4

#define LOG_SERIAL_DISPLAY (LOG_SERIAL | LOG_DISPLAY)
#define LOG_ALL (LOG_SERIAL | LOG_DISPLAY | LOG_SD)



class Log {
public:
    Log(HardwareSerial &serial, Display &display, SdFile &sdlog, GPRS &gprs) : serial(serial), display(display), sdlog(sdlog), gprs(gprs) {
    }

    void log(const String &msg, int dest=LOG_ALL, bool attach_timestamp=true);
    void log(const char *msg, int dest=LOG_ALL, bool attach_timestamp=true);


private:
    HardwareSerial &serial;
    Display &display;
    SdFile &sdlog;
    GPRS &gprs;

    String current_time_millis();
    const String empty_log_timestamp = "                                ";
    String attach_timestamp_to_msg(const String &msg, const String &millis_s, const String &time_s, bool is_regular);

};


#endif
