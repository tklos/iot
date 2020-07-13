# NodeMCU O<sub>2</sub> and CO<sub>2</sub> sensor

A NodeMCU-based air quality module. Measurement data are shown on the display and sent to the collect server (https://github.com/tklos/collect).



## Hardware

* NodeMCU v3
* MH-Z19 CO<sub>2</sub> sensor
* 0.96'' I2C OLED display
* (optional) 5V regulated power supply



## Wiring

MH-Z19 pin | Connected to
------------|:------------
Vin | NodeMCU 5V pin or 5V power supply
GND | GND
PWM | NodeMCU pin 12 (D6)
Tx | NodeMCU pin 13 (D7)
Rx | NodeMCU pin 15 (D8)


Display pin | Connected to
-----------------|:-------------
GND | GND
VDD | NodeMCU 3.3V pin
SCK | NodeMCU pin 5 (D1, SCL)
SDA | NodeMCU pin 4 (D2, SDA)



## Installation

* Create `air-quality/settings.cpp` based on its `.template` version
* Upload sketch

