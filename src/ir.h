#pragma once
#include <Arduino.h>
#include <IRremote.h>
#include <vector>

class IR
{
private:
  typedef void (*key_press_cb)(uint8_t);
  uint8_t pin;
  std::vector<key_press_cb> callbacks;
  void dispatch_key_pressed(uint8_t key_code) {
    for (auto cb : callbacks) {
      cb(key_code);
    }
  }

public:
  enum KeyCodes {
    num_1 = 69,
    num_2 = 70,
    num_3 = 71,
    num_4 = 68,
    num_5 = 64,
    num_6 = 67,
    num_7 = 7,
    num_8 = 21,
    num_9 = 9,
    num_0 = 25,
    star = 22,
    hash = 13,
    ok = 28,
    up = 24,
    down = 82,
    left = 8,
    right = 90
  };

  IR(uint8_t p): pin(p){}

  void begin() {
    IrReceiver.begin(pin);
    IrReceiver.enableIRIn();
  }

  uint32_t read_freq() {
    if (IrReceiver.decode()) {
      auto value = IrReceiver.decodedIRData.command;
      IrReceiver.resume();
      return value;
    }

    return 0;
  }

  void tick() {
    uint32_t freq = read_freq();
    if (freq) {
      dispatch_key_pressed(freq);
    }
  }

  void push_callback(key_press_cb cb) {
    callbacks.push_back(cb);
  }
};

IR MyIR(GPIO_NUM_23);
