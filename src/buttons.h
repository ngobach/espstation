#ifndef BUTTONS_H
#define BUTTONS_H
#include <Arduino.h>

class Button {
  private:
    int pin;
    int current_value;
    int last_value;
  public:
    Button(uint8_t p): pin(p), current_value(HIGH), last_value(HIGH) {}

    bool is_up() {
      return current_value != last_value && current_value == HIGH;
    }

    bool is_down() {
      return current_value != last_value && current_value == HIGH;
    }

    uint8_t get_current() {
      return current_value;
    }

    void update() {
      int new_value = digitalRead(pin);
      last_value = current_value;
      current_value = new_value;
    }
};

#endif