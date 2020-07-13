# NodeMCU O<sub>2</sub> and CO<sub>2</sub> sensor

A NodeMCU-based air quality module. Measurement data are shown on the display and sent to the collect server (https://github.com/tklos/collect).



## Hardware

* NodeMCU v3
* MH-Z19 CO<sub>2</sub> sensor
* ME2-O2-Ф20 O<sub>2</sub> sensor
* 0.96'' I2C OLED display
* 5V regulated power supply



## Wiring

MH-Z19 pin | Connected to
-----------|:------------
Vin | 5V power supply
GND | GND
Tx | NodeMCU pin 13 (D7)
Rx | NodeMCU pin 15 (D8)


ME2-O2-Ф20 pin | Connected to
---------------|:------------
GND | GND
VCC | 5V power supply
SIG | NodeMCU pin A0


Display pin | Connected to
------------|:------------
GND | GND
VDD | NodeMCU 3.3V pin
SCK | NodeMCU pin 5 (D1, SCL)
SDA | NodeMCU pin 4 (D2, SDA)



## Installation

* Create `air-quality/settings.cpp` based on its `.template` version
* Upload sketch

