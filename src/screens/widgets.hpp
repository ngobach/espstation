#pragma once

#include "base.hpp"

namespace appui
{
  class TextWidget : public Widget
  {
  public:
    String text;
    String sub;
    int32_t icon = -1;

    Rect measure(U8G2 *g) override
    {
      g->setFont(get_font_default());
      auto result = Rect{
          0,
          0,
          g->getWidth(),
          4 + g->getMaxCharHeight(),
      };

      if (!sub.isEmpty()) {
        g->setFont(get_font_small());
        result.h += g->getMaxCharHeight();
      }

      return result;
    }

    void paint(U8G2 *g, Rect r, bool focused) override
    {
      int yoffset = r.y + 1;

      if (!sub.isEmpty()) {
        g->setFont(get_font_small());
        g->drawStr(r.x + 2, yoffset + g->getMaxCharHeight(), sub.c_str());
        yoffset += g->getMaxCharHeight() + 1;
      }

      g->setFont(get_font_default());
      g->drawStr(r.x + 2, yoffset + g->getMaxCharHeight(), text.c_str());
      yoffset += g->getMaxCharHeight();

      if (focused) {
        g->drawRFrame(r.x, r.y, r.w, r.h, 2);
      }
    }
  };
}