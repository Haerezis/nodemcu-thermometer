#ifndef __STRUCT_SENSOR_DATA__
#define __STRUCT_SENSOR_DATA__

#include <DHT.h> //Temp & Humidity


struct SensorData {
  SensorData(char dht_pin, float t_bias = 0.0f, float h_bias = 0.0f);

  void begin();

  bool update_temperature_and_humidify();
  bool update_battery_level();

  String to_json();

  DHT dht;

  float temperature;
  float temperature_bias;
  float humidity;
  float humidity_bias;
  float battery_level;
};

#endif
