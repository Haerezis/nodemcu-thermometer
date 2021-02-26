#include "Test.hpp"

#include <LittleFS.h> //Filesystem via Flash memory
#include <ESP8266WiFi.h>

#include "Configuration.hpp"
#include "SensorData.hpp"
#include "Screen.hpp"
#include "Functions.hpp"

#define DHT_PIN D3

/////////////////////////////////////////////////////////
// CONFIGURATION
////////////////////////////////////////////////////////

void test_conf_1() {
  Configuration conf;
  Serial.println(conf.to_json());

  conf.mqtt_ip = "42.42.42.42";
  conf.mqtt_port = "4242";
  conf.mqtt_topic = "topic/foobar";
  conf.local_ip = "1.2.3.4";
  conf.local_netmask = "5.6.7.8";
  conf.local_gateway = "9.10.11.12";
  conf.temperature_bias = 42.0f;
  conf.humidity_bias = 42.42f;
  
  Serial.println(conf.to_json());

  if (conf.save_to_flash()) {
    Serial.println("SUCCESS");
  }
  else {
    Serial.println("FAILURE");
  }
}

void test_conf_2() {
  Configuration conf;
  Serial.println(conf.to_json());

  Serial.println("starting...");
  
  conf.load_from_flash();
  
  if (conf.load_successful) {
    Serial.println("SUCCESS");
    Serial.println(conf.to_json());
  }
  else {
    Serial.println("FAILURE");
    Serial.println(conf.to_json());
  }
}


void test_conf_3() {
  Configuration conf;
  Serial.println(conf.to_json());

  Serial.println("starting...");
  LittleFS.begin();
  LittleFS.remove(conf.file_path);
  LittleFS.end();

  conf.load_from_flash();

  if (conf.load_successful) {
    Serial.println("SUCCESS");
    Serial.println(conf.to_json());
  }
  else {
    Serial.println("FAILURE");
    Serial.println(conf.to_json());
  }
}


/////////////////////////////////////////////////////////
// SENSOR_DATA
////////////////////////////////////////////////////////

void test_sensor_data_1() {
  SensorData sd(DHT_PIN);

  sd.begin();

  Serial.println("SensorData json :");
  Serial.println(sd.to_json());

  Serial.println("Updating temp & humidify value...");
  if(sd.update_temperature_and_humidify()) {
    Serial.println("SUCCESS");
  }
  else {
    Serial.println("FAILURE");
  }

  Serial.println("SensorData json :");
  Serial.println(sd.to_json());
}

/////////////////////////////////////////////////////////
// SCREEN
////////////////////////////////////////////////////////

void test_screen_1() {
  Screen screen(
    D5,
    D6,
    D7,
    D8,
    D9,
    -1
  );

  SensorData data(DHT_PIN);
  data.update_battery_level();
   
  screen.begin();
  screen.display_normal_mode(data);
}

void test_screen_2() {
  Screen screen(
    D5,
    D6,
    D7,
    D8,
    D9,
    -1
  );
  
  screen.begin();
   
  String ap_ssid = String("AP_") + String(RANDOM_REG32);
  String ap_password = String(RANDOM_REG32);
  screen.display_config_mode_infos(ap_ssid, ap_password);
}

void test_screen_3() {
  Screen screen(
    D5,
    D6,
    D7,
    D8,
    D9,
    -1
  );
  
  screen.begin();

  screen.display_config_mode_success();
}

void test_screen_4() {
  Screen screen(
    D5,
    D6,
    D7,
    D8,
    D9,
    -1
  );
   
  screen.begin();

  String msg("Foobar foobar");
  screen.display_error(42, msg);
}

/////////////////////////////////////////////////////////
// MQTT
////////////////////////////////////////////////////////
void test_mqtt() {
  Configuration conf;
  SensorData data(DHT_PIN);

  conf.mqtt_ip = "192.168.0.42";
  conf.mqtt_port = "1883";
  conf.mqtt_topic = "dev/testing";
  Serial.println("Init");

  WiFi.begin("The Blind Forest", "ping_pong_is_the_best_anime_ever"); 
  while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected"); 

  data.update_temperature_and_humidify();
  data.update_battery_level();
  
  Serial.println(send_data_to_mqtt(conf, data));
  Serial.println("DONE");
}
