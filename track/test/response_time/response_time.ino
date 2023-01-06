
int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;


unsigned long start_time;
String msg = "", response = "";



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
	delay(3000);

	SerialUSB.begin(115200);
	Serial1.begin(115200);

	SerialUSB.println("Ready");
}



void loop() {
	/* Read from console */
	while (SerialUSB.available() > 0) {
		int c = SerialUSB.read();
        msg += (char)c;
        if (c == 13) {
            Serial1.write(msg.c_str());
            Serial1.flush();
            start_time = micros();
            msg = "";
            response = "";
        }
    }

	/* Read from GPRS */
	while (Serial1.available() > 0) {
        unsigned long time = micros() - start_time;
        int c = Serial1.read();
        if (c == 10)
            response += "$";
        else if (c == 13)
            response += "%";
        else
            response += (char)c;

        SerialUSB.print(response);
        SerialUSB.print(" ");
        SerialUSB.println((float)time/1000);
    }
}
