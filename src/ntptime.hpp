#pragma once

#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

class NtpTime_
{
private:
  WiFiUDP ntpUdp;
  NTPClient ntp = NTPClient(ntpUdp, 7 * 60 * 60);

  void __tick() {
    ntp.update();
  }

public:
  void begin() {
    ntp.setUpdateInterval(60 * 1000);
    ntp.begin();
    MainLoops.emplace_back([this] { __tick(); });
  }

  String get_time_string() {
    return ntp.getFormattedTime();
  }

  String get_date_string() {
    time_t epoch = ntp.getEpochTime();
    tm *now = localtime(&epoch);
    char buffer[32];
    sprintf(buffer, "%04d-%02d-%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);

    return String(buffer);
  }
};

NtpTime_ NtpTime;