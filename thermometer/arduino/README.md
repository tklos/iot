# Arduino thermometer


## Hardware
* Arduino Nano
* DS18B20 waterproof thermometer
* ESP-01S WiFi module
* 5V regulated power supply
* 5V to 3.3V power regulator, e.g. AMS1117
* 5V to 3.3V logic level converter



## Wiring

DS18B20 pin | Connected to
------------|:------------
VDD | 5V power supply
DATA | Arduino pin 5 (D2)
DATA | 5V power supply (via 4.7kΩ resistor)
GND | GND


ESP-01S pin | Connected to
--------|:-------------
VCC | 3.3V power supply
RX | Arduino pin 14 (D11) (via logic level converter)
RST | 3.3V power supply
CH_PD | 3.3V power supply
TX | Arduino pin 13 (D10)
GND | GND


Additionally, if Arduino is not connected to the computer by the USB cable, 5V power supply should be connected to Arduino pin 27 (+5V).



## Installation

### ESP-01S

* Prepare Arduino IDE: <https://github.com/esp8266/Arduino#installing-with-boards-manager>

* Wiring

   ESP-01S pin | Connected to
   --------|:-------------
   VCC | 3.3V power supply
   RX | Arduino pin 2 (RX)
   CH_PD | 3.3V power supply
   GPIO0 | GND
   TX | Arduino pin 1 (TX)
   GND | GND

   Additionally, connect Arduino RESET pin to GND.

* Create `devices/esp8266/settings.cpp` based on its `.template` version

* Upload `devices/esp8266/esp8266.ino` sketch to the ESP-01S


### Arduino

* Install libraries listed in `devices/arduino/libraries.txt`
* Create `devices/arduino/settings.cpp` based on its `.template` version
* Upload `devices/arduino/arduino.ino` sketch to Arduino

