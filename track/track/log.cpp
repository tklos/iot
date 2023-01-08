#include "log.h"



String Log::current_time_millis() {
    unsigned long milli = millis() / 1000;
    int second = milli % 60;
    milli /= 60;
    int minute = milli % 60;
    milli /= 60;
    int hour = milli;

    char buf[15];
    sprintf(buf, "%02d:%02d:%02d", hour, minute, second);
    return buf;
}


String Log::attach_timestamp_to_msg(const String &msg, const String &millis_s, const String &time_s, bool is_regular) {
    if (is_regular) {
        String msg_(msg);
        msg_.replace("\n", String("\n") + empty_log_timestamp);
        return millis_s + "  " + time_s + "  " + msg_;
    } else {
        return millis_s + "\n" + time_s + "\n" + msg;
    }
}


void Log::log(const String &msg, int dest, bool attach_timestamp) {
    String msg_, msg_display;
    if (attach_timestamp) {
        String millis_s = current_time_millis();
        String time_s = gprs.current_time();

        msg_ = attach_timestamp_to_msg(msg, millis_s, time_s, true);
        msg_display = attach_timestamp_to_msg(msg, millis_s, time_s, false);
    } else {
        msg_ = msg_display = msg;
    }

    if (dest & LOG_SERIAL)
        serial.println(msg_);

    if (dest & LOG_DISPLAY)
        display.print(msg_display);

    if (dest & LOG_SD) {
        sdlog.println(msg_);
        sdlog.sync();
    }
}


void Log::log(const char *msg, int dest, bool attach_timestamp) {
    String msg_ = msg;
    log(msg_, dest, attach_timestamp);
}
