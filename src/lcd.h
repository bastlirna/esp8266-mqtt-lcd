#ifndef __LCD_H
#define __LCD_H

#include <U8g2lib.h>
#include <ArduinoJson.h>

class LcdClass{

public:
    LcdClass(U8G2_ST7565_64128N_F_4W_HW_SPI &_lcd, uint8_t _backlight_pin) : lcd(_lcd), backlight_pin(_backlight_pin) {}
    void begin();
    bool display(JsonObject& input, JsonObject& output);

protected:
    bool drawItem(JsonObject& json, JsonObject& output);
private:
    U8G2_ST7565_64128N_F_4W_HW_SPI &lcd;
    uint8_t backlight_pin;

};



#endif
