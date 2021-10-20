#pragma once

#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Ticker.h>

class NtpTime_
{
private:
  WiFiUDP ntpUdp;
  NTPClient ntp = NTPClient(ntpUdp, 7 * 60 * 60);
  Ticker ticker;

  void __tick() {
    ntp.update();
  }

public:
  void begin() {
    ntp.setUpdateInterval(5000);
    ntp.begin();
    ticker.attach_ms(100, [this] { __tick(); });
  }

  String get_time_string() {
    return ntp.getFormattedTime();
  }
};

NtpTime_ NtpTime;