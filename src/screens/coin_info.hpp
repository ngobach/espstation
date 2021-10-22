#pragma once

#include "base.hpp"
#include "widgets.hpp"

namespace appui
{
  class CoinInfoScreen : public Vlist
  {
  private:
    String symbol;

  public:
    CoinInfoScreen(ScreenParams params): Vlist()
    {
      if (params->front().first != "symbol") {
        return;
      }

      symbol = params->front().second;
      auto tw = std::make_unique<TextWidget>();
      tw->text = symbol;

      items.emplace_back(std::move(tw));
    }

    void key_pressed(KeyCode kc, bool is_long) override {
      if (is_long && kc == KeyCode::Sub) {
        switch_screen(ScreenName::Coins);
        return;
      }

      Vlist::key_pressed(kc, is_long);
    }
  };
}