#include "gprs.h"


static char BUF[BUFLEN];


void GPRS::begin() {
    serial.begin(baud);
}


bool GPRS::check_ok(const char *buf) {
    return !strncmp(buf, "OK", 2);
}


bool GPRS::check_ok_anywhere(const char *buf) {
    return check_ok(buf) || strstr(buf, "\x0D\x0AOK");
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

    return check_ok(buf);
}


bool GPRS::send_cmd_check_ok_anywhere(const char *cmd, unsigned long wait_time, unsigned int timeout) {
    char *buf = send_cmd_return(cmd, timeout);
    if (!buf)
        return false;

    return check_ok_anywhere(buf);
}


int GPRS::num_smses() {
    char *ret = send_cmd_return("AT+CPMS?");
    if (!check_ok_anywhere(ret))
        return -1;

    char *tok = strtok(ret, ",");
    if (!tok)
        return -1;

    tok = strtok(NULL, ",");
    return atoi(tok);
}


char *GPRS::read_sms() {
    /* Output of AT+CMGR consists of:
     * header \13\10
     * text (possibly with \10)
     * \13\10\13\10
     */
    char *ret = send_cmd_return("AT+CMGR=1");
    if (!ret)
        return NULL;

    char *tok = strstr(ret, "\xD\xA");
    if (!tok)
        return NULL;
    tok += 2;

    char *end = strstr(tok, "\xD\xA");
    if (!end)
        return NULL;
    *end = '\0';

    return tok;
}


bool GPRS::delete_sms() {
    bool ret = send_cmd_check_ok("AT+CMGD=1");
    return ret;
}


String GPRS::current_date() {
    char *ret = send_cmd_return("AT+CCLK?");
    if (!ret)
        return "000000";

    String s = ret;

    if (s.length() < 30 || !s.startsWith("+CCLK: "))
        return "000000";

    return s.substring(8, 10) + s.substring(11, 13) + s.substring(14, 16);
}


String GPRS::current_time() {
    char *ret = send_cmd_return("AT+CCLK?");
    if (!ret)
        return "      ????????      ";

    String s = ret;

    if (s.length() < 30 || !s.startsWith("+CCLK: "))
        return "      ????????      ";

    return s.substring(8, 28);
}
