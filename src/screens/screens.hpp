#pragma once

#include <memory>
#include <Arduino.h>
#include "ui.hpp"
#include "loading.hpp"
#include "menu.hpp"
#include "coins.hpp"
#include "coin_info.hpp"
#include "test.hpp"

namespace appui
{
  std::unique_ptr<Screen> load_screen(ScreenName name, ScreenParams params = nullptr)
  {
    switch (name)
    {
    case ScreenName::Loading:
      return std::make_unique<LoadingScreen>();

    case ScreenName::Coins:
      return std::make_unique<CoinsScreen>();

    case ScreenName::CoinInfo:
      Serial.println("loading CoinInfoScreen");
      return std::make_unique<CoinInfoScreen>(std::move(params));

    case ScreenName::Menu:
      return std::make_unique<MenuScreen>();

    case ScreenName::Test:
      return std::make_unique<TestScreen>();

    default:
      return nullptr;
    }
  }
}