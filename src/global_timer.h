#include <arduino-timer.h>

extern Timer<> global_timer;

#ifndef GLOBAL_TIMER_H
#define GLOBAL_TIMER_H

Timer<> global_timer = Timer<>();

#endif