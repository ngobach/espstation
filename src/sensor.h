#include <DHT.h>
#include <DHT_U.h>

const byte DHT_TYPE = DHT11;
const byte DHT_PAD = GPIO_NUM_18;

struct SensorRead {
  float temperature;
  float relativeHumidity;
};

class Sensor {
  private:
    DHT_Unified sensor;
  public:
    Sensor();
    void begin();
    SensorRead read();
};

#ifndef SENSOR_IMPLEMENTATION
#define SENSOR_IMPLEMENTATION

Sensor::Sensor(): sensor(DHT_PAD, DHT_TYPE) {}

void Sensor::begin() {

}

SensorRead Sensor::read() {

}
#endif