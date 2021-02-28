#ifndef __STRUCT_CONFIGURATION__
#define __STRUCT_CONFIGURATION__

#include <WString.h>

struct Configuration {
  Configuration();

  bool save_to_flash();
  void load_from_flash();
  String to_json();
  
  String file_path;
  bool load_successful;

  String local_ip;
  String local_netmask;
  String local_gateway;
  
  String mqtt_ip;
  String mqtt_port;
  String mqtt_topic;

  float temperature_bias;
  float humidity_bias;

  //unit : seconds
  uint32_t sleep_duration;
};

#endif
