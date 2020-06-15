# NodeMCU thermometer

A NodeMCU-based thermometer. Measurement data are shown on the display and sent to the collect server (https://github.com/tklos/collect).



## Hardware

* NodeMCU v3
* DS18B20 waterproof thermometer
* 0.96'' I2C OLED display
* (optional) 5V regulated power supply



## Wiring

DS18B20 pin | Connected to
------------|:------------
VDD | NodeMCU 3.3V pin or 5V power supply
DATA | NodeMCU pin 12 (D6)
DATA | Power supply (via 4.7kΩ resistor)
GND | GND

Display pin | Connected to
-----------------|:-------------
GND | GND
VDD | NodeMCU 3.3V pin
SCK | NodeMCU pin 5 (D1, SCL)
SDA | NodeMCU pin 4 (D2, SDA)



## Installation

* Install `OneWire`, `DallasTemperature` and `Adafruit_SSD1306` libraries
* Create `thermometer/settings.cpp` based on its `.template` version
* Upload sketch

