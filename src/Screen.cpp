#include "Screen.hpp"

#include <HardwareSerial.h>
#include <math.h>

// 'layout2', 84x48px
const unsigned char layout_bitmap [] PROGMEM = {
  0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x0f, 0x80, 0x44, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x04, 0x43, 0xf0, 0x0f, 0x80, 0x57, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x47, 0xf8, 0x7f, 
  0xf0, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x4e, 0x18, 0x7f, 0xf0, 0x57, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0x9c, 0x00, 0x60, 0x30, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 
  0x60, 0x30, 0x57, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x60, 0x30, 0x54, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x60, 0x30, 0xd6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 
  0x00, 0x60, 0x30, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x18, 0x60, 0x30, 0xba, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x60, 0x30, 0xba, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x03, 0xf0, 0x60, 0x30, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x7c, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
  0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x60, 0x30, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x10, 0x00, 0x00, 
  0x00, 0x70, 0x40, 0x00, 0x00, 0x00, 0x60, 0x30, 0x38, 0x00, 0x00, 0x00, 0x50, 0xc0, 0x00, 0x00, 
  0x00, 0x60, 0x30, 0x6c, 0x00, 0x00, 0x00, 0x71, 0x80, 0x00, 0x00, 0x00, 0x60, 0x30, 0x44, 0x00, 
  0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x8a, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 
  0x00, 0x00, 0x60, 0x30, 0x8a, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x9a, 
  0x00, 0x00, 0x00, 0x19, 0xc0, 0x00, 0x00, 0x00, 0x60, 0x30, 0xc6, 0x00, 0x00, 0x00, 0x31, 0x40, 
  0x00, 0x00, 0x00, 0x7f, 0xf0, 0x7c, 0x00, 0x00, 0x00, 0x61, 0xc0, 0x00, 0x00, 0x00, 0x7f, 0xf0
};




Screen::Screen(int8_t SCLK, int8_t DIN, int8_t DC, int8_t CS, int8_t RST, int8_t BL) :
  display(SCLK, DIN, DC, CS, RST),
  backlight_pin(BL) {
  if (backlight_pin > 0) {
    pinMode(backlight_pin, OUTPUT);
    backlight_off();
  }
}



Screen::Screen(int8_t DC, int8_t CS, int8_t RST, int8_t BL) :
  display(DC, CS, RST),
  backlight_pin(BL) {
  if (backlight_pin > 0) {
    pinMode(backlight_pin, OUTPUT);
    backlight_off();
  }
}



void Screen::begin() {
  //clear adafruit default bitmap
  display.clearDisplay();
  display.begin();
  display.setContrast(40);
}



void Screen::backlight_on() {
  if (backlight_pin <= 0) {
    return;
  }
  if(backlight_on_when_pin_high) {
    digitalWrite(backlight_pin, HIGH);
  }
  else {
    digitalWrite(backlight_pin, LOW);
  }
}


void Screen::backlight_off() {
  if (backlight_pin <= 0) {
    return;
  }
  if(backlight_on_when_pin_high) {
    digitalWrite(backlight_pin, LOW);
  }
  else {
    digitalWrite(backlight_pin, HIGH);
  }
}



void Screen::draw_title(const String & title) {
  display.setTextSize(1);
  display.setTextWrap(false);

  display.fillRect(0, 0, 84, 9, BLACK);

  uint8_t offset = ((84 - (title.length() * 6 - 1)) / 2);
  display.setCursor(offset,1);
  display.setTextColor(WHITE, BLACK);
  display.println(title);
  
  display.setCursor(0,10);
}



void Screen::display_normal_mode(const SensorData & data) {
  display.clearDisplay();

  // BATTERY LEVEL
  uint8_t relative_battery_level = (data.battery_level / 100.0) * 40;
  if (relative_battery_level > 0) {
    display.fillRect(
      76,
      5 + (40 - relative_battery_level),
      5,
      relative_battery_level,
      BLACK
    );
  }

  // TEMPERATURE
  float temp = fabs(data.temperature);
  uint8_t temp_intpart = temp;
  uint8_t temp_fracpart = (temp - temp_intpart) * 10;
  
  int8_t temp1 = temp_intpart / 10;
  int8_t temp2 = temp_intpart % 10;
  int8_t temp3 = temp_fracpart;

  if (data.temperature < 0) {
    if (temp1 > 0) {
      display.drawChar(
        -1,
        12,
        '-',
        BLACK,
        WHITE,
        2
      );
    }
    else {
      display.drawChar(
        18,
        12,
        '-',
        BLACK,
        WHITE,
        2
      );
    }
  }

  if (temp1 > 0) {
    display.drawChar(
      10,
      0,
      '0' + temp1,
      BLACK,
      WHITE,
      4
    );
  }

  display.drawChar(
    10 + 5*4 + 1,
    0,
    '0' + temp2,
    BLACK,
    WHITE,
    4
  );

  if (temp3 > 0) {
    display.drawChar(
      58,
      14,
      '0' + temp3,
      BLACK,
      WHITE,
      2
    );
  }

  // HUMIDITY
  int8_t hum1 = abs((data.humidity / 10));
  int8_t hum2 = abs((((int8_t)data.humidity) % 10));

  if (hum1 > 0) {
    display.drawChar(
      10,
      34,
      '0' + hum1,
      BLACK,
      WHITE,
      2
    );
  }

  display.drawChar(
    10 + 5*2 + 1,
    34,
    '0' + hum2,
    BLACK,
    WHITE,
    2
  );

  //LAYOUT
  display.drawBitmap(
    0, 0,
    layout_bitmap,
    84, 48,
    BLACK
  );
  display.display();
}



void Screen::display_config_mode_infos(const String & ssid, const String & password) {
  display.clearDisplay();

  draw_title("CONFIGURATION");

  display.setTextSize(1);
  display.setTextWrap(false);
  display.setTextColor(BLACK, WHITE);

  display.println("Wifi:");
  display.println(ssid);
  display.println("Password:");
  display.println(password);

  display.display();
}



void Screen::display_config_mode_success() {
  display.clearDisplay();

  draw_title("SUCCESS");

  display.setTextSize(1);
  display.setTextWrap(false);
  display.setTextColor(BLACK, WHITE);

  display.println("Switch to normal");
  display.println("mode, and reboot");
  display.println("device.");

  display.display();
}



void Screen::display_error(uint16_t code, const String & msg) {
  display.clearDisplay();
  
  draw_title("ERROR");

  display.setTextSize(1);
  display.setTextWrap(true);
  display.setTextColor(BLACK, WHITE);

  display.print("Code: ");
  display.println(code);
  if (msg.length() > 0) {
    display.println(msg);
  }

  display.display();
}
