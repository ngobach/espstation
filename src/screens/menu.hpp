#pragma once

#include "base.hpp"

namespace appui {
  struct MenuScreenItem
  {
    const char *title;
    const char *icon;
    const ScreenName screen;
  };

  class MenuScreen : public Screen
  {
  private:
    const MenuScreenItem items[2] = {
        {
            .title = "Coins",
            .icon = "\u0229",
            .screen = ScreenName::Coins,
        },
        {
            .title = "System",
            .icon = "\u01fc",
            .screen = ScreenName::Loading,
        },
    };
    const size_t items_count = sizeof(items) / sizeof(MenuScreenItem);
    int current_index = 0;

  public:
    void paint(U8G2 *g) override
    {
      const MenuScreenItem &current = items[current_index];
      draw_title(g, current.title);
      char pos[20];
      sprintf(pos, "%d/%d", current_index + 1, items_count);
      g->setFont(get_font_small());
      g->drawStr((g->getWidth() - g->getStrWidth(pos)) / 2, 7, pos);
      g->setFont(get_font_icon_21());
      g->drawUTF8((g->getWidth() - 21) / 2, (g->getHeight() - 10 + 8 + 21) / 2, current.icon);
    }

    void key_pressed(KeyCode kc, bool is_long) override
    {
      if (is_long && kc == KeyCode::Main)
      {
        Serial.printf("Loading screen %d\n", kc);
        switch_screen(items[current_index].screen);
        return;
      }

      if (!is_long)
      {
        switch (kc)
        {
        case KeyCode::Main:
          update([this]
                 { current_index = (current_index - 1 + items_count) % items_count; });
          break;
        case KeyCode::Sub:
          update([this]
                 { current_index = (current_index + 1) % items_count; });
          break;
        }
      }
    }
  };
}