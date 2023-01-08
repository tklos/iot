#include <Arduino.h>

#include "settings.h"
#include "gprs.h"
#include "display.h"
#include "sd.h"
#include "log.h"
#include "metro.h"


const int PWR_KEY = 9;
const int RST_KEY = 6;
const int LOW_PWR_KEY = 5;

const int SD_CS = 4;

HardwareSerial &serial = SerialUSB;
HardwareSerial &serial_gprs = Serial1;

GPRS gprs(serial_gprs, baud_gprs);
Display display;
SdFile sdlog;

Log logger(serial, display, sdlog, gprs);
Metro metro_sms(INTERVAL_SMS);


#define BUFLEN 255
char buf[BUFLEN];


/* Status */
bool gps_on = false;



void setuperror() {
    logger.log("Error", LOG_ALL, false);
    while (true) {
    }
}


void setup() {
    /* Device setup */
    pinMode(PWR_KEY, OUTPUT);
    pinMode(RST_KEY, OUTPUT);
    pinMode(LOW_PWR_KEY, OUTPUT);
    digitalWrite(RST_KEY, LOW);
    digitalWrite(LOW_PWR_KEY, HIGH);
    digitalWrite(PWR_KEY, HIGH);
    digitalWrite(PWR_KEY, LOW);
    delay(3000);
    digitalWrite(PWR_KEY, HIGH);
    delay(5000);

    serial.begin(baud_monitor);
    gprs.begin();
    if (!display.begin())
        logger.log("Display initialization failed", LOG_SERIAL, false);

    logger.log("Initialisation finished, starting setup..", LOG_SERIAL_DISPLAY, false);


    /* GPRS */
    logger.log("Switch off echo", LOG_SERIAL_DISPLAY, false);
    bool ret = gprs.send_cmd_check_ok_anywhere("ATE0", 40);
    if (!ret)
        setuperror();

    logger.log("AT", LOG_SERIAL_DISPLAY, false);
    ret = gprs.send_cmd_check_ok("AT");
    if (!ret)
        setuperror();

    logger.log("Disable unsolicited network registration messages", LOG_SERIAL_DISPLAY, false);
    ret = gprs.send_cmd_check_ok("AT+CREG=0");
    if (!ret)
        setuperror();

    logger.log("Switch off new SMS indications", LOG_SERIAL_DISPLAY, false);
    ret = gprs.send_cmd_check_ok("AT+CNMI=0,0,0,0,0");
    if (!ret)
        setuperror();

    logger.log("Switch off indications", LOG_SERIAL_DISPLAY, false);
    ret = gprs.send_cmd_check_ok("AT+CMER=3,0,0,0");
    if (!ret)
        setuperror();

    logger.log("Set SMS format to text", LOG_SERIAL_DISPLAY, false);
    ret = gprs.send_cmd_check_ok("AT+CMGF=1");
    if (!ret)
        setuperror();

    logger.log("Set SMS storage", LOG_SERIAL_DISPLAY, false);
    ret = gprs.send_cmd_check_ok_anywhere("AT+CPMS=\"SM\",\"SM\",\"SM\"");
    if (!ret)
        setuperror();

    serial.print("Wait for network registration ");
    logger.log("Wait for network registration", LOG_DISPLAY, false);
    while (true) {
        char *ret = gprs.send_cmd_return("AT+CREG?");
        if (
            ret &&
            gprs.check_ok_anywhere(ret) &&
            (!strncmp(ret, "+CREG: 0,1", 10) || !strncmp(ret, "+CREG: 0,5", 10))
        )
            break;
        serial.print(".");
        delay(500);
    };
    serial.println();

    logger.log("Connected", LOG_SERIAL_DISPLAY, false);


    /* Data */
    logger.log("Activate Packet Data System", LOG_SERIAL_DISPLAY, false);
    ret = gprs.send_cmd_check_ok_anywhere("AT+CGATT=1");
    if (!ret)
        setuperror();

    logger.log("Setup Packet Data Protocol", LOG_SERIAL_DISPLAY, false);
    String cmd = String("") + "AT+CGDCONT=1,\"IP\",\"" + apn + "\"";
    ret = gprs.send_cmd_check_ok(cmd.c_str());
    if (!ret)
        setuperror();

    logger.log("Activate Packet Data Protocol", LOG_SERIAL_DISPLAY, false);
    ret = gprs.send_cmd_check_ok("AT+CGACT=1");
    if (!ret)
        setuperror();


    /* SD card */
    logger.log("Initialise SD card", LOG_SERIAL_DISPLAY, false);
    if (!sd.begin(SD_CS, SPI_HALF_SPEED))
        setuperror();

    String current_date = gprs.current_date();
    if (!create_log_dir(log_dir)) {
        logger.log("Can't create log dir", LOG_SERIAL_DISPLAY, false);
        setuperror();
    }

    String filename = get_log_filename(log_dir, current_date);
    logger.log(String("") + "Creating log file: \"" + filename + "\"", LOG_SERIAL_DISPLAY, false);
    if (filename == "") {
        logger.log("Empty log filename", LOG_SERIAL_DISPLAY, false);
        setuperror();
    }
    if (!create_log_file(sdlog, filename)) {
        logger.log(String("") + "Can't create log file \"" + filename + "\"", LOG_SERIAL_DISPLAY, false);
        setuperror();
    }


    /* Metro */
    metro_sms.start();


    logger.log("Setup finished", LOG_SERIAL_DISPLAY, false);
}



void loop() {
	if (metro_sms.check())
		check_sms();

    /* Read from console */
    while (serial.available() > 0) {
        serial_gprs.write(serial.read());
    }
    /* Read from GPRS */
    while (serial_gprs.available() > 0) {
        serial.write(serial_gprs.read());
    }
}



void check_sms() {
    int num_smses = gprs.num_smses();
    logger.log(String("") + "Read " + num_smses + " SMSes", LOG_SERIAL);
    if (num_smses <= 0)
        return;

    process_sms();
}


void process_cmd_status() {
    String msg = "Processing status";
    msg += String("\n") + "GPS: " + gps_on;

    logger.log(msg);
}


void process_cmd_gps_on() {
    logger.log("Processing GPS on");

    if (gps_on) {
        logger.log("Already on, ignoring..");
        return;
    }

    bool ret = gprs.gps_on();
    if (!ret) {
        logger.log("Switching GPS on failed");
        return;
    }

    gps_on = true;
    logger.log("GPS now on");
}


void process_cmd_gps_off() {
    logger.log("Processing GPS off");

    if (!gps_on) {
        logger.log("Already off, ignoring..");
        return;
    }

    bool ret = gprs.gps_off();
    if (!ret) {
        logger.log("Switching GPS off failed");
        return;
    }

    gps_on = false;
    logger.log("GPS now off");
}


void process_cmd_gps_loc() {
    logger.log("Processing GPS loc");

    if (!gps_on) {
        /* Try switching GPS on */
        logger.log("GPS off, switching on..");
        process_cmd_gps_on();
        if (!gps_on) {
            logger.log("Can't switch GPS on, quitting..");
            return;
        }
    }

    /* Get GPS location */
    char *ret = gprs.get_location();
    if (!ret) {
        logger.log("Can't get GPS location, quitting..");
        return;
    }
    strncpy(buf, ret, BUFLEN);
    logger.log(String("") + "Got GPS location: " + buf);

    /* Send GPS location */
    String data = String("") + "%5B" + buf + "%5D";
    bool ret2 = gprs.send_post(collect_url, api_key, data.c_str());
    if (!ret2) {
        logger.log("Sending GPS location failed, quitting..");
        return;
    }
    logger.log("GPS location sent successfully");
}


void process_sms() {
    logger.log("Received SMS");
    char *msg_s = gprs.read_sms();
    if (!msg_s) {
        logger.log("Can't read SMS");
        return;
    }

    String msg = msg_s;
    msg.toLowerCase();

    if (msg.startsWith("status"))
        process_cmd_status();
    else if (msg.startsWith("gpson"))
        process_cmd_gps_on();
    else if (msg.startsWith("gpsoff"))
        process_cmd_gps_off();
    else if (msg.startsWith("gpsloc"))
        process_cmd_gps_loc();
    else
        logger.log(String("") + "Can't understand SMS message \"" + msg + "\"");

    gprs.delete_sms();
}
