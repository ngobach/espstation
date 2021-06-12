#include <Adafruit_SSD1306.h>

class Display
{
private:
  Adafruit_SSD1306 *d;
  unsigned long lastDisplay;
  unsigned long startAt;

public:
  Display();
  ~Display();
  bool begin();
  bool tick();
};
