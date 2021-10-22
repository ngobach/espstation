#pragma once

#include "vlist.hpp"
#include "widgets.hpp"
#include "corelib.hpp"

namespace appui
{
  class CoinsScreen : public Vlist
  {
  private:
    const uint32_t FETCH_INTERVAL = 60 * 1000;
    uint32_t last_fetch = 0;
    bool is_fetching = false;
    std::vector<core::Coin> coins;

  public:
    void on_tick() override
    {
      uint32_t now = millis();

      if (is_fetching)
      {
        update([this, now]
               {
                 coins = CoreLib.get_coins();
                 items.clear();
                 items.reserve(coins.size());
                 char buffer[24];
                 for (auto &c : coins)
                 {
                   auto tw = std::make_unique<TextWidget>();
                   sprintf(buffer, "%s (%s)", c.name, c.symbol);
                   tw->sub = String(buffer);
                   sprintf(buffer, "%.4f", c.current_price);
                   tw->text = String(buffer);
                   items.emplace_back(std::move(tw));
                 }
                 is_fetching = false;
               });
        return;
      }

      if (!last_fetch || now - last_fetch >= FETCH_INTERVAL)
      {
        update([this, now]
               {
                 is_fetching = true;
                 last_fetch = now;
               });
        return;
      }
    }

    void paint(U8G2 *g) override
    {
      Vlist::paint(g);
      if (is_fetching)
      {
        draw_title(g, "Fetching...");
      }
    }

    void key_pressed(KeyCode kc, bool is_long) override
    {
      Vlist::key_pressed(kc, is_long);

      if (is_long && kc == KeyCode::Sub)
      {
        switch_screen(ScreenName::Menu);
        return;
      }

      if (is_long && kc == KeyCode::Main) {
        auto params = std::make_unique<std::vector<std::pair<String, String>>>();
        params->emplace_back("symbol", coins[focused_index].symbol);
        switch_screen(ScreenName::CoinInfo, std::move(params));
      }
    }
  };
}