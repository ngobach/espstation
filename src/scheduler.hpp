#pragma once

#include "TickerScheduler.h"

TickerScheduler TS(8);
std::vector<std::function<void()>> MainLoops;