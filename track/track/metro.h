/* Inspired by Thomas Ouellet Fredericks's Metro library:
 *  https://github.com/thomasfredericks/Metro-Arduino-Wiring
 */

#ifndef __metro_h__
#define __metro_h__



class Metro {
public:
    Metro(unsigned long interval): interval(interval) {
        previous_time = millis() - interval;
    }

    void start();
    void reset(unsigned long interval);

    bool check();

    unsigned long get_interval_s();


private:
    unsigned long interval;
    unsigned long previous_time;

};


#endif
