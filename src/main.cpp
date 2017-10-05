#include <Homie.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

#include "lcd.h"
#include "keywords.h"

#define BACKLIGHT_PIN 5

#ifndef JSON_BUFFER_SIZE
#define JSON_BUFFER_SIZE 2048
#endif

U8G2_ST7565_64128N_F_4W_HW_SPI u8g2(U8G2_R0, 4, 2 , 15);
static LcdClass lcd(u8g2, BACKLIGHT_PIN);
HomieNode contentNode("content", "json");
/*
bool globalInputHandler(const HomieNode& node, const String& property, const HomieRange& range, const String& value) {
  
    Homie.getLogger() << "Received on node " << node.getId() << ": " << property << " = " << value << endl;
    return true;
  }
*/

bool contentHandler(const HomieRange& range, const String& value) {
  StaticJsonBuffer<JSON_BUFFER_SIZE> inputJsonBuffer;
  DynamicJsonBuffer outputJsonBuffer;
  
  JsonObject& input = inputJsonBuffer.parseObject(value);
  JsonObject& output = outputJsonBuffer.createObject();

  if (input.success()) {
    uint32_t stime = millis();
    if(lcd.display(input, output)){
      output[KEYWORD_STATUS] = KEYWORD_STATUS_OK;
    }else{
      output[KEYWORD_STATUS] = KEYWORD_STATUS_FAILED;
    }
    output[KEYWORD_TIME] = millis() - stime;
  }else{
    Homie.getLogger() << "parseObject() failed: " << value << endl;
    output[KEYWORD_STATUS] = KEYWORD_STATUS_FAILED;
    output[KEYWORD_ERROR] = KEYWORD_ERROR_INVALID_JSON;
  }

  String outputString;
  output.printTo(outputString);
  contentNode.setProperty("status").send(outputString);

  return true;
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  WiFi.disconnect(); // Homie 2.0 issue #351 hack

  delay(1000);
  
  Homie.getLogger() << "Init LCD" << endl;
  lcd.begin();

  delay(1000);
  Homie.getLogger() << "Init Homie" << endl;

  Homie.setResetTrigger(12, LOW, 2000);

  Homie.disableLedFeedback();
  Homie_setFirmware("MQTTLCD_generic", "1.0.0");
  
  contentNode.advertise("content").settable(contentHandler);
  //Homie.setGlobalInputHandler(globalInputHandler);
  Homie.setup();

  Homie.getLogger() << "Init done" << endl;
}

void loop() {

  Homie.loop();
}
