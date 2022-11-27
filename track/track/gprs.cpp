#include "gprs.h"


static char BUF[BUFLEN];


void GPRS::begin() {
    serial.begin(baud);
}


void GPRS::send_cmd(const char *cmd) {
    /* Clear response buffer before sending new command */
    while (serial.available())
        serial.read();

    serial.write(cmd);
    serial.write("\r\n");
    serial.flush();
}


bool read_response(HardwareSerial &serial, char *response, unsigned int timeout) {
    unsigned long start_time = millis();

    int num_read = 0;
    while (true) {
        bool read_in_this_loop = false;  // have we read any characters in this iteration?
        while (serial.available()) {
            int c = serial.read();
            if (num_read > 0 || (c != 10 && c != 13)) {
                response[num_read] = c;
                num_read++;
            }
            read_in_this_loop = true;
        }
        if (read_in_this_loop) {
            delayMicroseconds(DELAY_READ_US);
            continue;
        }

        if (num_read) {
            response[num_read] = '\0';
            return true;
        }
        if (millis() - start_time > timeout)
            return false;
    }
}


char *GPRS::send_cmd_return(const char *cmd, unsigned long wait_time, unsigned int timeout) {
    /* Send `cmd` and wait for response, up until `timeout` ms.
     *
     * Returns:
     *     pointer to statically allocated buffer
     *         if response arrived within `timeout` ms
     *     NULL
     *         otherwise
    */
    send_cmd(cmd);

    if (wait_time)
        delay(wait_time);

    if (read_response(serial, BUF, timeout))
        return BUF;
    else
        return NULL;
}


bool GPRS::send_cmd_check_ok(const char *cmd, unsigned long wait_time, unsigned int timeout) {
    /* Send `cmd` and check if "OK" is returned */
    char *buf = send_cmd_return(cmd, wait_time, timeout);
    if (!buf)
        return false;

    return !strncmp(buf, "OK", 2);
}


bool GPRS::send_cmd_check_ok_anywhere(const char *cmd, unsigned long wait_time, unsigned int timeout) {
    char *buf = send_cmd_return(cmd, timeout);
    if (!buf)
        return false;

    return !strncmp(buf, "OK", 2) || strstr(buf, "\x0D\x0AOK");
}
