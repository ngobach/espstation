#include <DHT.h>
#include <DHT_U.h>
#include <Ticker.h>

const uint8_t DHT_TYPE = DHT11;
const uint8_t DHT_PAD = GPIO_NUM_18;
const uint32_t SENSOR_READ_INTERVAL = 1500;

struct SensorRead {
  float temperature;
  float relative_humidity;
};

class Sensor {
  private:
    DHT_Unified sensor;
    SensorRead last_value;
    Ticker tkr;

  public:
    Sensor();
    void begin();
    void read();
    SensorRead get();
};

extern Sensor MySensor;

#ifndef SENSOR_IMPLEMENTATION
#define SENSOR_IMPLEMENTATION

Sensor::Sensor(): sensor(DHT_PAD, DHT_TYPE) {}

void Sensor::begin() {
  this->sensor.begin();
  this->tkr.attach_ms<Sensor *>(SENSOR_READ_INTERVAL, [](Sensor *s) {
    s->read();
  }, this);
  this->read();
}

void Sensor::read() {
  SensorRead value;
  sensors_event_t tmp;
  this->sensor.temperature().getEvent(&tmp);
  value.temperature = tmp.temperature;
  this->sensor.humidity().getEvent(&tmp);
  value.relative_humidity = tmp.relative_humidity;
  this->last_value = value;
}

SensorRead Sensor::get() {
  return this->last_value;
}

Sensor MySensor;
#endif