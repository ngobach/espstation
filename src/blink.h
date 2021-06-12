class Blink
{
private:
  int pin;
  int interval;
  long lastBlink = 0;
  bool isOn;
public:
  Blink(int pin, int interval);
  ~Blink();
  bool begin();
  bool tick();
};