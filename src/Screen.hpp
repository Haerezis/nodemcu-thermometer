#ifndef __CLASS_SCREEN__
#define __CLASS_SCREEN__

#include <WiFiManager.h>
#include <Adafruit_PCD8544.h>

#include "SensorData.hpp"
#include "Configuration.hpp"

class Screen {
public:
  Screen(int8_t SCLK, int8_t DIN, int8_t DC, int8_t CS, int8_t RST, int8_t BL);
  Screen(int8_t DC, int8_t CS, int8_t RST, int8_t BL);

  void begin();

  void backlight_on();
  void backlight_off();

  void draw_title(const String & title);
  
  void display_normal_mode(const SensorData & data);
  void display_config_mode_infos(const String & ssid,const String & password);
  void display_config_mode_success();
  void display_error(uint16_t code, const String & msg = "");
  

protected:
  Adafruit_PCD8544 display;
  int8_t backlight_pin;
  bool backlight_on_when_pin_high = true;
};

#endif
