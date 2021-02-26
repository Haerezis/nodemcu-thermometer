#include "SensorData.hpp"

#include <HardwareSerial.h>
#include <WString.h>
#include <ArduinoJson.h>

SensorData::SensorData(char dht_pin, float t_bias, float h_bias) :
  dht(dht_pin, DHT11),
  temperature(0.0f),
  temperature_bias(t_bias),
  humidity(0.0f),
  humidity_bias(h_bias),
  battery_level(0.0f) {
}

void SensorData::begin() {
  this->dht.begin();
}

bool SensorData::update_temperature_and_humidify() {
  float t = this->dht.readTemperature();
  float h = this->dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return false;
  }

  this->temperature = t + this->temperature_bias;
  this->humidity = h + this->humidity_bias;

  return true;
}

bool SensorData::SensorData::update_battery_level() {
  //TODO
  this->battery_level = 42.0f;

  return true;
}

String SensorData::SensorData::to_json() {
  DynamicJsonDocument doc(256);
  String retval;

  doc["temperature"] = this->temperature;
  doc["humidity"] = this->humidity;
  doc["battery_level"] = this->battery_level;

  serializeJson(doc, retval);

  return retval;
}
