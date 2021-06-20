#ifndef SCREEN_LOADER_H
#define SCREEN_LOADER_H
#include "screens/splash.h"
#include "screens/clock.h"

enum ScreenNames {
  splash,
  clock,
  weather,
  internet_weather
};

Screen* load_screen(ScreenNames screen_name) {
  switch (screen_name) {
    case ScreenNames::splash:
      return new SplashScreen;
    case ScreenNames::clock:
      return new ClockScreen;
    default:
      panic("Invalid screen name");
      return nullptr;
  }
}
#endif