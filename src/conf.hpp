#pragma once

struct AppConfigType {
  const char* WifiName;
  const char* WifiPassword;
};

const AppConfigType AppConfig = {
  .WifiName = "BachNX",
  .WifiPassword = "idontgiveafuck",
};
