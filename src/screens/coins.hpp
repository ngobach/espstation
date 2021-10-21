#pragma once

#include "base.hpp"
#include "corelib.hpp"

namespace appui
{

  class CoinsScreen : public Screen
  {
  private:
    uint32_t last_fetch = -1;
    bool is_fetching = false;
    std::vector<core::Coin> coins;
    int page_offset = 0, next_page_offset = INT_MIN;
    size_t focused_index = 0;

  protected:
    void on_tick() override
    {
      int32_t now = millis();
      if (is_fetching)
      {
        update([this, now]
               {
                 coins = CoreLib.get_coins();
                 is_fetching = false;
               });
      }
      if (now - last_fetch > 5000)
      {
        update([this, now]
               {
                 is_fetching = true;
                 last_fetch = now;
               });
      }
      if (next_page_offset != INT_MIN)
      {
        update([this]
               {
                 page_offset = next_page_offset;
                 next_page_offset = INT_MIN;
               });
      }
    }

    int draw_coin(U8G2 *g, int y, core::Coin &coin, bool bordered)
    {
      char title[32], price[32];
      int yoffset;
      sprintf(title, "%s (%s)", coin.name, coin.symbol);
      sprintf(price, "%0.2f", coin.current_price);
      g->setFont(get_font_small());
      yoffset = y + 2 + g->getMaxCharHeight();
      g->drawStr(4, yoffset, title);
      g->setFont(get_font_default());
      yoffset += 1 + g->getMaxCharHeight();
      g->drawStr(4, yoffset, price);
      yoffset += 2;
      if (bordered)
      {
        g->drawRFrame(0, y, g->getDisplayWidth(), yoffset - y, 2);
      }
      return yoffset - y;
    }

    void paint(U8G2 *g) override
    {
      if (is_fetching)
      {
        draw_title(g, "Fetching...");
        return;
      }

      int offset = page_offset;
      for (size_t i = 0; i < coins.size(); i++)
      {
        int h = draw_coin(g, offset, coins[i], i == focused_index);

        if (i == focused_index)
        {
          if (offset < 0)
          {
            next_page_offset = page_offset + (-offset);
          }
          else if (offset + h + 2 > g->getDisplayHeight())
          {
            next_page_offset = page_offset - (offset + h + 2 - g->getDisplayHeight());
          }
        }
        offset += h + 2;
      }
    }

    void key_pressed(KeyCode kc, bool is_long) override
    {
      if (is_fetching)
      {
        return;
      }

      if (!is_long)
      {
        if (kc == KeyCode::Sub && focused_index != coins.size() - 1)
        {
          update([this]
                 { focused_index = focused_index + 1; });
        }
        else if (kc == KeyCode::Main && focused_index > 0)
        {
          update([this]
                 { focused_index = focused_index - 1; });
        }
      }
      else
      {
        if (kc == KeyCode::Sub)
        {
          switch_screen(ScreenName::Menu);
        }
      }
    }
  };
}