# NodeMCU thermometer


## Hardware

* NodeMCU v3
* DS18B20 waterproof thermometer
* 5V regulated power supply (optional)



## Wiring

DS18B20 pin | Connected to
------------|:------------
VDD | NodeMCU 3.3V pin or 5V power supply
DATA | NodeMCU pin 5 (D1)
DATA | Power supply (via 4.7kΩ resistor)
GND | GND



## Installation

* Install `OneWire` and `DallasTemperature` libraries
* Create `thermometer/settings.cpp` based on its `.template` version
* Upload sketch

