#ifndef SENSOR_H
#define SENSOR_H
#include <Ticker.h>
#include <DHT.h>
#include <DHT_U.h>

struct SensorRead
{
  float temperature;
  float relative_humidity;
};

class Sensor
{
private:
  const uint8_t DHT_TYPE = DHT11;
  const uint8_t DHT_PAD = GPIO_NUM_18;
  const uint32_t TICKER_INTERVAL = 2000;
  DHT_Unified sensor;
  SensorRead last_value;
  Ticker tkr;

public:
  Sensor() : sensor(DHT_PAD, DHT_TYPE) {}

  void begin()
  {
    this->sensor.begin();
    this->tkr.attach_ms<Sensor *>(
        TICKER_INTERVAL, [](Sensor *s)
        { s->read(); },
        this);
    this->read();
  }

  void read()
  {
    SensorRead value;
    sensors_event_t tmp;
    this->sensor.temperature().getEvent(&tmp);
    value.temperature = tmp.temperature;
    this->sensor.humidity().getEvent(&tmp);
    value.relative_humidity = tmp.relative_humidity;
    this->last_value = value;
  }

  SensorRead get()
  {
    return this->last_value;
  }
};

Sensor MySensor;
#endif