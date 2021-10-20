#pragma once

#include <memory>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

namespace httpc {
  WiFiClient socket;
  String get(const char *url) {
    HTTPClient http_client;
    http_client.begin(socket, url);

    if (http_client.GET() < 0) {
      return "";
    }

    String response = http_client.getString();
    
    return response;
  }
}

namespace core
{
  struct Coin {
    char name[20];
    char symbol[8];
    double current_price;
  };

  class CoreLib_
  {
    public:
    std::vector<Coin> get_coins() {
      std::vector<Coin> coins;
      String response = httpc::get("http://espstation.app.matmang.xyz/coins");

      if (response.length()) {
        DynamicJsonDocument doc(response.length() * 2);
        deserializeJson(doc, response.c_str());
        coins.reserve(doc.size());
        for (size_t i = 0; i < doc.size(); i++) {
          JsonObject obj = doc[i];
          Coin coin;
          strcpy(coin.name, obj["Name"].as<const char*>());
          strcpy(coin.symbol, obj["Symbol"].as<const char*>());
          coin.current_price = obj["CurrentPrice"].as<double>();
          coins.emplace_back(coin);
        }
      }

      return coins;
    }
  };
}

core::CoreLib_ CoreLib;
