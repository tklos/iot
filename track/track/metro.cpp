#include <Arduino.h>

#include "metro.h"



void Metro::start() {
    previous_time = millis() - interval;
}


void Metro::reset(unsigned long interval) {
    this->interval = interval;
    start();
}


bool Metro::check() {
    unsigned long curr_time = millis();
    unsigned long diff = curr_time - previous_time;
    if (diff >= interval) {
        unsigned long num_intervals = diff / interval;
        previous_time += num_intervals * interval;

        return true;
    }

    return false;
}


unsigned long Metro::get_interval_s() {
    return interval / 1000;
}
