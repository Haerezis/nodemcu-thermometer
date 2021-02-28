#include "Configuration.hpp"

#include <LittleFS.h> //Filesystem via Flash memory
#include <ArduinoJson.h>
#include <HardwareSerial.h>

Configuration::Configuration() : file_path("/config.json"), load_successful(false), mqtt_port("1883") {
}

bool Configuration::save_to_flash() {
  LittleFS.begin();
  File config_file = LittleFS.open(this->file_path, "w");
  
  if (!config_file) {
    Serial.println("failed to open config file for writing");
    LittleFS.end();
    return false;
  }

  config_file.print(to_json());

  config_file.close();
  LittleFS.end();

  return true;
}

void Configuration::load_from_flash() {
  LittleFS.begin();

  File config_file = LittleFS.open(this->file_path, "r");
  if (!config_file) {
    Serial.println("failed to open config file for writing");
    this->load_successful = false;
    return;
  }

  size_t size = config_file.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  config_file.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  auto err = deserializeJson(doc, buf.get());
  if (err) {
    Serial.println("failed to parse config file as json");
    this->load_successful = false;
    return;
  }

  this->mqtt_ip = doc["mqtt_ip"].as<String>();
  this->mqtt_port = doc["mqtt_port"].as<String>();
  this->mqtt_topic = doc["mqtt_topic"].as<String>();

  this->local_ip = doc["local_ip"].as<String>();
  this->local_netmask = doc["local_netmask"].as<String>();
  this->local_gateway = doc["local_gateway"].as<String>();

  this->temperature_bias = doc["temperature_bias"];
  this->humidity_bias = doc["humidity_bias"];

  this->sleep_duration = doc["sleep_duration"];

  this->load_successful = true;

  config_file.close();

  LittleFS.end();
}

String Configuration::to_json() {
  DynamicJsonDocument doc(256);
  String retval;

  doc["mqtt_ip"] = this->mqtt_ip;
  doc["mqtt_port"] = this->mqtt_port;
  doc["mqtt_topic"] = this->mqtt_topic;

  doc["local_ip"] = this->local_ip;
  doc["local_netmask"] = this->local_netmask;
  doc["local_gateway"] = this->local_gateway;

  doc["temperature_bias"] = this->temperature_bias;
  doc["humidity_bias"] = this->humidity_bias;

  doc["sleep_duration"] = this->sleep_duration;

  serializeJson(doc, retval);

  return retval;
}
