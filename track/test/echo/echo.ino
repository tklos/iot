/* Forward keyboard commands to/from GPRS */

int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;



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
		Serial1.write(SerialUSB.read());
	}
	/* Read from GPRS */
	while (Serial1.available() > 0) {
		SerialUSB.write(Serial1.read());
	}
}
