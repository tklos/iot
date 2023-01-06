#include "gprs.h"


static char BUF[BUFLEN];
static char HTTPBUF[1024];


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
    /* Read data from `serial` and copy it to `response`.
     *
     * Omits leading bytes 10 and 13. Appends byte 0 to `response`.
     *
     * Stops reading when there is nothing to read for `DELAY_READ_US` us or when timeout of `timeout` ms has been reached.
     *
     * Returns:
     *     false
     *         if timeout has been reached
     *     true
     *         otherwise
    */
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
    return send_cmd_return_r(cmd, BUF, wait_time, timeout);
}


char *GPRS::send_cmd_return_r(const char *cmd, char *buf, unsigned long wait_time, unsigned int timeout) {
    send_cmd(cmd);

    if (wait_time)
        delay(wait_time);

    if (read_response(serial, buf, timeout))
        return buf;
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
    char *buf = send_cmd_return(cmd, wait_time, timeout);
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


bool GPRS::gps_on() {
    bool ret = send_cmd_check_ok("AT+GPS=1");
    return ret;
}


bool GPRS::gps_off() {
    bool ret = send_cmd_check_ok("AT+GPS=0");
    return ret;
}


char *GPRS::get_location() {
    /* Get GPS latitude and longitude.
     *
     * Returns:
     *     pointer to statically allocated buffer with comma-separated latitude and longitude values
     *     NULL
     *         if we couldn't get GPS fix
    */
    char *ret = send_cmd_return("AT+LOCATION=2");
    if (!ret)
        return NULL;

    if (strstr(ret, "GPS NOT FIX NOW"))
        return NULL;
    if (!check_ok_anywhere(ret))
        return NULL;

    char *tok = strstr(ret, "\xD\xA");
    if (!tok)
        return NULL;
    *tok = '\0';

    return ret;
}


bool GPRS::send_post(const char *post_url, const char *api_key, const char *data) {
    String cmd = String("") + "AT+HTTPPOST=\"" + post_url + "\",\"application/x-www-form-urlencoded\",\"api-key=" + api_key + "&data=" + data + "\"";

    char *ret = send_cmd_return_r(cmd.c_str(), HTTPBUF, 0, TIMEOUT_CMD_SEND_POST);
    if (!ret)
        return false;
    if (!check_ok(ret))
        return false;

    /* There usually is a small (~20 ms) gap between receiving the initial "OK" and the rest of the HTTP response (HTTP/1.1  201  Created ... {"status":"ok"}).
     * We don't really need to have the full response. */
    return true;
}
