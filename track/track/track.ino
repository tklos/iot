#include <Arduino.h>

#include "settings.h"
#include "gprs.h"
#include "sd.h"
#include "metro.h"


const int PWR_KEY = 9;
const int RST_KEY = 6;
const int LOW_PWR_KEY = 5;

const int SD_CS = 4;

HardwareSerial &serial = SerialUSB;
HardwareSerial &serial_gprs = Serial1;

GPRS gprs(serial_gprs, baud_gprs);

SdFile sdlog;

Metro metro_sms(INTERVAL_SMS);


#define BUFLEN 255
char buf[BUFLEN];


/* Status */
bool gps_on = false;



void setuperror() {
    serial.println("Error");
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

    serial.println("Ready");


    /* GPRS */
    serial.println("Switch off echo");
    bool ret = gprs.send_cmd_check_ok_anywhere("ATE0", 40);
    if (!ret)
        setuperror();

    serial.println("AT");
    ret = gprs.send_cmd_check_ok("AT");
    if (!ret)
        setuperror();

    serial.println("Disable unsolicited network registration messages");
    ret = gprs.send_cmd_check_ok("AT+CREG=0");
    if (!ret)
        setuperror();

    serial.println("Switch off new SMS indications");
    ret = gprs.send_cmd_check_ok("AT+CNMI=0,0,0,0,0");
    if (!ret)
        setuperror();

    serial.println("Switch off indications");
    ret = gprs.send_cmd_check_ok("AT+CMER=3,0,0,0");
    if (!ret)
        setuperror();

    serial.println("Set SMS format to text");
    ret = gprs.send_cmd_check_ok("AT+CMGF=1");
    if (!ret)
        setuperror();

    serial.println("Set SMS storage");
    ret = gprs.send_cmd_check_ok_anywhere("AT+CPMS=\"SM\",\"SM\",\"SM\"");
    if (!ret)
        setuperror();

    serial.print("Wait for network registration ");
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

    serial.println("Connected");


    /* Data */
    serial.println("Activate Packet Data System");
    ret = gprs.send_cmd_check_ok_anywhere("AT+CGATT=1");
    if (!ret)
        setuperror();

    serial.println("Setup Packet Data Protocol");
    String cmd = String("") + "AT+CGDCONT=1,\"IP\",\"" + apn + "\"";
    ret = gprs.send_cmd_check_ok(cmd.c_str());
    if (!ret)
        setuperror();

    serial.println("Activate Packet Data Protocol");
    ret = gprs.send_cmd_check_ok("AT+CGACT=1");
    if (!ret)
        setuperror();


    /* SD card */
    serial.println("Initialise SD card");
    if (!sd.begin(SD_CS, SPI_HALF_SPEED))
        setuperror();

    String current_date = gprs.current_date();
    if (!create_log_dir(log_dir)) {
        serial.println("Can't create log dir");
        setuperror();
    }

    String filename = get_log_filename(log_dir, current_date);
    serial.println(String("Creating log file: \"") + filename + "\"");
    if (filename == "")
        setuperror();

    if (!create_log_file(sdlog, filename))
        setuperror();


    /* Metro */
    metro_sms.start();


    serial.println("Setup finished");
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



String current_time_millis() {
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


String get_log_timestamp() {
    return current_time_millis() + "  " + gprs.current_time() + "  ";
}

const String empty_log_timestamp = "                                ";


String attach_timestamp_to_msg(const String &msg) {
    String msg_(msg);
    msg_.replace("\n", String("\n") + empty_log_timestamp);
    return get_log_timestamp() + msg_;
}


void log(const String &msg) {
    /* Print `msg` to both serial and sdlog. Attach timestamp. */
    String msg_ = attach_timestamp_to_msg(msg);

    serial.println(msg_);
    sdlog.println(msg_);
    sdlog.sync();
}


void log(const char *msg) {
    String msg_ = msg;
    log(msg_);
}


void log_serial(const String &msg) {
    serial.println(attach_timestamp_to_msg(msg));
}


void log_serial(const char *msg) {
    String msg_ = msg;
    log_serial(msg_);
}


void check_sms() {
    int num_smses = gprs.num_smses();
    log_serial(String("Read ") + num_smses + " SMSes");
    if (num_smses <= 0)
        return;

    process_sms();
}


void process_cmd_status() {
    String msg = "Processing status";
    msg += String("\n") + "GPS: " + gps_on;

    log(msg);
}


void process_cmd_gps_on() {
    log("Processing GPS on");

    if (gps_on) {
        log("Already on, ignoring..");
        return;
    }

    bool ret = gprs.gps_on();
    if (!ret) {
        log("Switching GPS on failed");
        return;
    }

    gps_on = true;
    log("GPS now on");
}


void process_cmd_gps_off() {
    log("Processing GPS off");

    if (!gps_on) {
        log("Already off, ignoring..");
        return;
    }

    bool ret = gprs.gps_off();
    if (!ret) {
        log("Switching GPS off failed");
        return;
    }

    gps_on = false;
    log("GPS now off");
}


void process_cmd_gps_loc() {
    log("Processing GPS loc");

    if (!gps_on) {
        /* Try switching GPS on */
        log("GPS off, switching on..");
        process_cmd_gps_on();
        if (!gps_on) {
            log("Can't switch GPS on, quitting..");
            return;
        }
    }

    /* Get GPS location */
    char *ret = gprs.get_location();
    if (!ret) {
        log("Can't get GPS location, quitting..");
        return;
    }
    strncpy(buf, ret, BUFLEN);
    log(String("") + "Got GPS location: " + buf);

    /* Send GPS location */
    String data = String("") + "%5B" + buf + "%5D";
    bool ret2 = gprs.send_post(collect_url, api_key, data.c_str());
    if (!ret2) {
        log("Sending GPS location failed, quitting..");
        return;
    }
    log("GPS location sent successfully");
}


void process_sms() {
    log("Received SMS");
    char *msg_s = gprs.read_sms();
    if (!msg_s) {
        log("Can't read SMS");
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
        log(String("Can't understand SMS message \"") + msg + "\"");

    gprs.delete_sms();
}
