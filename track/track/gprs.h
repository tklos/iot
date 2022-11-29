#ifndef __gprs_h__
#define __gprs_h__

#include <Arduino.h>


#define BUFLEN 255

#define TIMEOUT_CMD_DEFAULT 5000

#define DELAY_READ_US 90  // Time to send 1 byte is 10/115200 s ~= 87 us


class GPRS {
public:
    GPRS(HardwareSerial &serial, unsigned int baud) : serial(serial) {
        this->baud = baud;
    }

    void begin();

    bool check_ok(const char *buf);
    bool check_ok_anywhere(const char *buf);

    void send_cmd(const char *cmd);
    char *send_cmd_return(const char *cmd, unsigned long wait_time=0, unsigned int timeout=TIMEOUT_CMD_DEFAULT);
    bool send_cmd_check_ok(const char *cmd, unsigned long wait_time=0, unsigned int timeout=TIMEOUT_CMD_DEFAULT);
    bool send_cmd_check_ok_anywhere(const char *cmd, unsigned long wait_time=0, unsigned int timeout=TIMEOUT_CMD_DEFAULT);

    int num_smses();
    char *read_sms();
    bool delete_sms();

    String current_date();
    String current_time();

    bool gps_on();
    bool gps_off();


private:
    HardwareSerial &serial;
    long baud;

};


#endif
