#include "DHT11.h"

#include <future>

#if BUILD_DHT11
#include <pi_2_dht_read.h>
#undef DHT11
#endif

#define DHT_SENSOR_TYPE 11
#define DHT_SENSOR_PIN 12

DHT11::DHT11(QObject* parent)
  : QObject(parent)
{}

void
DHT11::update()
{
#if BUILD_DHT11
    std::async([&]() {
        float temperature = 0, humidity = 0;
        if (pi_2_dht_read(DHT_SENSOR_TYPE, DHT_SENSOR_PIN, &humidity, &temperature) ==
            DHT_SUCCESS) {
            m_temperature = temperature;
            m_humidity = humidity;
        }
    });
#endif
}
