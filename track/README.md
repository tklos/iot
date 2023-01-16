# GPS tracking with Maduino


## Hardware

* Maduino Zero A9G: https://www.makerfabs.com/maduino-zero-a9g.html
* 0.96" I2C OLED display
* microSIM card with data plan
* microSD card
* 5V USB micro-B power supply


## Installation

* Insert SIM and SD cards and connect display
* Install software for the Maduino board in Arduino IDE as described at https://wiki.makerfabs.com/Maduino_Zero_A9G.html
* Create `track/settings.cpp` based on its `.template` version
* Upload sketch


## How it works

The device can be controlled by sending SMS messages to its mobile number. Measurements are sent to the collect server by mobile data service.

**Note:** There is a bug in A9G's firmware that allows to send only 7 *HTTPS* requests without restarting the module.
The problem is best explained at https://stackoverflow.com/questions/68612918/https-requests-on-a9g-via-at-commands-fail-after-7-requests-http-works-fine.
We'll be using HTTP instead.


## SMS commands

All commands are case-insensitive. Log massages are printed to serial monitor, display and file on SD card.

* **`status`:** Print basic status.
* **`gpson`:** Switch on GPS.
* **`gpsoff`:** Switch off GPS.
* **`gpsloc`:** Send a single GPS location.
* **`gpsstart (interval)`:** Start GPS tracking. `interval` is in seconds, optional, default `60`.
* **`gpsstop`:** Stop tracking.


## Power consumption

State | Power cons.
 --- | ---:
Connecting to network | 40 mA
Idle<br/>(connected to network) | 30 mA
GPS on | 75 mA
Tracking<br/>(one record per minute) | 80 mA


## Data consumption

A single record uses around 1.2 kB (HTTP) or 8.8 kB (HTTPS) of data.
One hour of tracking (one record per minute) is around 70 kB (HTTP).


## Future work

* Power saving / Sleep mode
* MQTT

