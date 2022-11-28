#include <Arduino.h>

#include "metro.h"



void Metro::start() {
    previous_time = millis() - interval;
}


void Metro::reset() {
    previous_time = millis() - interval;
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
