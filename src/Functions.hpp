#ifndef __FUNCTIONS__HPP
#define __FUNCTIONS__HPP

#include <WiFiManager.h>

#include "Configuration.hpp"
#include "SensorData.hpp"
#include "Screen.hpp"


#define ERROR__STARTUP__NO_MODE 101
#define ERROR__CONF__LOAD_FAILURE 201
#define ERROR__CONF__SAVE_FAILURE 202
#define ERROR__WIFI__NOTHING_SAVED 301
#define ERROR__WIFI__FAILURE_TO_CONNECT 302
#define ERROR__MQTT__FAILURE_TO_SEND 401

IPAddress IPAddressFromString(const String & str);
int connect_to_wifi(Configuration & conf, WiFiManager & wm);
bool send_data_to_mqtt(Configuration & conf, SensorData & data);

void run_config_mode(Screen & screen, Configuration & conf);
void run_normal_mode(Screen & screen, Configuration & conf);
void run_error_mode(Screen & screen, Configuration & conf, uint8_t code, const String & msg = "");


#endif
