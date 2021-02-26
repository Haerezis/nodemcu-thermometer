#include "Functions.hpp"

#include <ESP8266WiFi.h>
#include <PubSubClient.h> //MQTT lib

IPAddress IPAddressFromString(const String & str) {
  IPAddress ip;
  
  ip.fromString(str);

  return ip;
}

int connect_to_wifi(Configuration & conf, WiFiManager & wm) {
  if (!wm.getWiFiIsSaved()) {
    Serial.println("No wifi already saved");
    return ERROR__WIFI__NOTHING_SAVED;
  }

  int retval = 0;

  wm.setEnableConfigPortal(false);

  wm.setSTAStaticIPConfig(
    IPAddressFromString(conf.local_ip),
    IPAddressFromString(conf.local_gateway),
    IPAddressFromString(conf.local_netmask)
  );

  if (!wm.autoConnect()) {
    retval = ERROR__WIFI__FAILURE_TO_CONNECT;
    Serial.println("failed to connect and hit timeout");
  }
  else {
    Serial.println("Wifi connected successfully");
  }

  return retval;
}

bool send_data_to_mqtt(Configuration & conf, SensorData & data) {
  bool retval = true;
  WiFiClient wifi_client;
  PubSubClient mqtt_client(conf.mqtt_ip.c_str(), conf.mqtt_port.toInt(), NULL, wifi_client);

  mqtt_client.connect(WiFi.macAddress().c_str());//Use chip MAC address as client ID (should be unique for each chip)
  retval = mqtt_client.publish(conf.mqtt_topic.c_str(), data.to_json().c_str());

  return retval;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MODE EXECUTION FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// CONFIG MODE
/////////////////////////////////////////////////////////

void run_config_mode(Screen & screen, Configuration & conf) {
  String ap_ssid = String("AP_") + String(RANDOM_REG32);
  String ap_password = String(RANDOM_REG32);
  WiFiManager wm;

  wm.setShowStaticFields(true);

  WiFiManagerParameter param_mqtt_ip("mqtt_ip", "MQTT server IP", conf.mqtt_ip.c_str(), 16);
  WiFiManagerParameter param_mqtt_port("mqtt_port", "MQTT server port", conf.mqtt_port.c_str(), 5);
  WiFiManagerParameter param_mqtt_topic("mqtt_topic", "MQTT topic", conf.mqtt_topic.c_str(), 256);
  WiFiManagerParameter param_temperature_bias("temperature_bias", "Temperature bias (float)", String(conf.temperature_bias).c_str(), 256);
  WiFiManagerParameter param_humidity_bias("humidity_bias", "Humidity bias (float)", String(conf.temperature_bias).c_str(), 256);

  wm.addParameter(&param_mqtt_ip);
  wm.addParameter(&param_mqtt_port);
  wm.addParameter(&param_mqtt_topic);
  wm.addParameter(&param_temperature_bias);
  wm.addParameter(&param_humidity_bias);

  wm.setSTAStaticIPConfig(
    IPAddressFromString(conf.local_ip),
    IPAddressFromString(conf.local_gateway),
    IPAddressFromString(conf.local_netmask)
  );

  screen.display_config_mode_infos(ap_ssid, ap_password);
  wm.startConfigPortal(ap_ssid.c_str(), ap_password.c_str());

  conf.mqtt_ip = String(param_mqtt_ip.getValue());
  conf.mqtt_port = String(param_mqtt_port.getValue());
  conf.mqtt_topic = String(param_mqtt_topic.getValue());
  conf.local_ip = WiFi.localIP().toString();
  conf.local_netmask = WiFi.subnetMask().toString();
  conf.local_gateway = WiFi.gatewayIP().toString();
  conf.temperature_bias = String(param_temperature_bias.getValue()).toFloat();
  conf.humidity_bias = String(param_humidity_bias.getValue()).toFloat();

  if(!conf.save_to_flash()) {
    run_error_mode(screen, conf, ERROR__CONF__SAVE_FAILURE, "Conf save failed");
    return;
  }
  
  screen.display_config_mode_success();
  ESP.deepSleep(0);
}


/////////////////////////////////////////////////////////
// NORMAL MODE
/////////////////////////////////////////////////////////

void run_normal_mode(Screen & screen, Configuration & conf) {
  //TODO
  /*
   * if config not loaded => run_error_mode
   * else
   *   connect wifi
   *   read temp
   *   send mqtt
   *   update display
   *   sleep
   */
   //ESP.deepsleep(15 * 60 * 1000000);
}


/////////////////////////////////////////////////////////
// ERROR MODE
/////////////////////////////////////////////////////////

void run_error_mode(Screen & screen, Configuration & conf, uint8_t code, const String & msg) {
  screen.display_error(code, msg);
  ESP.deepSleep(0);
}
