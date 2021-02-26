#include <ESP8266WiFi.h>

#include "Configuration.hpp"
#include "SensorData.hpp"
#include "Screen.hpp"
#include "Functions.hpp"

#include "Test.hpp"

#define CONFIG_MODE_PIN D1
#define NORMAL_MODE_PIN D2

void setup() {
  WiFi.setAutoConnect(false);
  
  Serial.begin(74880);
  Serial.println("\nStarting");

  pinMode(CONFIG_MODE_PIN, INPUT_PULLUP);
  pinMode(NORMAL_MODE_PIN, INPUT_PULLUP);

  Configuration conf;
  Screen screen(D5, D6, D7, D8, D9, -1);
  
  conf.load_from_flash();
  screen.begin();

  bool config_mode = digitalRead(CONFIG_MODE_PIN) == LOW;
  bool normal_mode = digitalRead(NORMAL_MODE_PIN) == LOW;

  if(config_mode) {
    run_config_mode(screen, conf);
  }
  else if(normal_mode) {
    run_normal_mode(screen, conf);
  }
  else {
    run_error_mode(screen, conf, ERROR__STARTUP__NO_MODE);
  }


  pinMode(LED_BUILTIN, OUTPUT);
  while(true) {
    Serial.println("loop");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1500);
  }
}

void loop() {
  delay(100000);
}
