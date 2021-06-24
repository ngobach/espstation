#pragma once
#include <Arduino.h>
#include <IRremote.h>
#include <map>
#include <functional>

class IR
{
private:
  typedef std::function<void(uint8_t)> key_press_cb;
  uint8_t pin;
  std::map<uint16_t, key_press_cb> callbacks;
  uint8_t last_value;
  void dispatch_key_pressed(uint8_t key_code) {
    for (auto &cb : callbacks) {
      cb.second(key_code);
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

  uint32_t read_value() {
    if (IrReceiver.decode()) {
      auto value = IrReceiver.decodedIRData.command;
      IrReceiver.resume();
      return value;
    }

    return 0;
  }

  void tick() {
    uint32_t value = read_value();
    if (value) {
      dispatch_key_pressed(value);
      last_value = value;
    } else {
      last_value = 0;
    }
  }

  uint16_t push_callback(key_press_cb cb) {
    while (true) {
      uint16_t key = random(999999);
      if (callbacks.find(key) == callbacks.end()) {
        callbacks[key] = cb;
        return key;
      }
    }
  }

  void remove_callback(uint16_t key) {
    callbacks.erase(callbacks.find(key));
  }

  uint8_t get_last_value() {
    return last_value;
  }
};

IR MyIR(GPIO_NUM_23);

int8_t keycode_to_value(IR::KeyCodes code) {
  switch (code) {
    case IR::KeyCodes::num_0:
      return 0;
    case IR::KeyCodes::num_1:
      return 1;
    case IR::KeyCodes::num_2:
      return 2;
    case IR::KeyCodes::num_3:
      return 3;
    case IR::KeyCodes::num_4:
      return 4;
    case IR::KeyCodes::num_5:
      return 5;
    case IR::KeyCodes::num_6:
      return 6;
    case IR::KeyCodes::num_7:
      return 7;
    case IR::KeyCodes::num_8:
      return 8;
    case IR::KeyCodes::num_9:
      return 9;
    default:
      return -1;
  }
}
