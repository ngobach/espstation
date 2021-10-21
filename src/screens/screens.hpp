#pragma once

#include <memory>
#include <Arduino.h>
#include "ui.hpp"
#include "loading.hpp"
#include "menu.hpp"
#include "coins.hpp"

namespace appui
{
  std::unique_ptr<Screen> load_screen(ScreenName name)
  {
    switch (name)
    {
    case ScreenName::Loading:
      return std::make_unique<LoadingScreen>();

    case ScreenName::Coins:
      return std::make_unique<CoinsScreen>();

    case ScreenName::Menu:
      return std::make_unique<MenuScreen>();

    default:
      return nullptr;
    }
  }
}