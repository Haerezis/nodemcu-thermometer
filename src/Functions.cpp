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

bool send_data_to_mqtt(Configuration & conf, SensorData & sensor) {
  bool retval = true;
  WiFiClient wifi_client;
  PubSubClient mqtt_client(conf.mqtt_ip.c_str(), conf.mqtt_port.toInt(), NULL, wifi_client);

  mqtt_client.connect(WiFi.macAddress().c_str());//Use chip MAC address as client ID (should be unique for each chip)
  retval = mqtt_client.publish(conf.mqtt_topic.c_str(), sensor.to_json().c_str());

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
  WiFiManagerParameter param_sleep_duration("sleep_duration", "Sleep duration between 2 readings (in seconds)", String(conf.sleep_duration).c_str(), 256);

  wm.addParameter(&param_mqtt_ip);
  wm.addParameter(&param_mqtt_port);
  wm.addParameter(&param_mqtt_topic);
  wm.addParameter(&param_temperature_bias);
  wm.addParameter(&param_humidity_bias);
  wm.addParameter(&param_sleep_duration);

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
  conf.sleep_duration = String(param_sleep_duration.getValue()).toInt();


  if(!conf.save_to_flash()) {
    run_error_mode(screen, conf, ERROR__CONF__SAVE_FAILURE, "Conf save failed");
    return;
  }

  screen.display_config_mode_success();
  
  //In success, display success on screen, and go into deep sleep indefinitevely.
  // => Waiting for the device to change mode (to normal mode) AND be manually rebooted
  ESP.deepSleep(0);
}


/////////////////////////////////////////////////////////
// NORMAL MODE
/////////////////////////////////////////////////////////

void run_normal_mode(Screen & screen, SensorData & sensor, Configuration & conf) {
  if (!conf.load_successful) {
    run_error_mode(screen, conf, ERROR__CONF__LOAD_FAILURE, "Conf load failed");
    return;
  }

   WiFiManager wm;

  int wifi_retval = connect_to_wifi(conf, wm);
  if (wifi_retval != 0) {
    run_error_mode(screen, conf, wifi_retval, "Wifi con. failed");
    return;
  }

  sensor.update_temperature_and_humidify();
  sensor.update_battery_level();

  bool mqtt_retval = send_data_to_mqtt(conf, sensor);
  if(!mqtt_retval) {
    run_error_mode(screen, conf, ERROR__MQTT__FAILURE_TO_SEND, "MQTT send failed");
    return;
  }

  screen.display_normal_mode(sensor);

  //conf.sleep_duration is in seconds, but ESP.deepSleep expects microseconds
  ESP.deepSleep(conf.sleep_duration * 1000000);
}


/////////////////////////////////////////////////////////
// ERROR MODE
/////////////////////////////////////////////////////////

void run_error_mode(Screen & screen, Configuration & conf, uint16_t code, const String & msg) {
  screen.display_error(code, msg);
  ESP.deepSleep(0);
}
