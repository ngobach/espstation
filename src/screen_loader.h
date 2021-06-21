#ifndef SCREEN_LOADER_H
#define SCREEN_LOADER_H
#include "screens/splash.h"
#include "screens/clock.h"
#include "screens/weather.h"
#include "screens/ir.h"

enum ScreenNames {
  splash,
  clock,
  weather,
  internet_weather,
  ir_tool
};

Screen* load_screen(ScreenNames screen_name) {
  switch (screen_name) {
    case ScreenNames::splash:
      return new SplashScreen;
    case ScreenNames::clock:
      return new ClockScreen;
    case ScreenNames::weather:
      return new WeatherScreen;
    case ScreenNames::ir_tool:
      return new IrScreen;
    default:
      panic("Invalid screen name");
      return nullptr;
  }
}
#endif