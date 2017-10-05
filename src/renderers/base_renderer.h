#ifndef __BASE_RENDERER_H
#define __BASE_RENDERER_H

#include <U8g2lib.h>
#include <ArduinoJson.h>

class BaseRenderer{
public:
    BaseRenderer(U8G2_ST7565_64128N_F_4W_HW_SPI& _lcd, JsonObject& _input, JsonObject& _output) : lcd(_lcd), inputJson(_input), outputJson(_output) {};
    virtual bool draw() = 0;
protected:
    U8G2_ST7565_64128N_F_4W_HW_SPI& lcd;
    JsonObject& inputJson;
    JsonObject& outputJson;

    template <typename ValueT> ValueT loadProperty(const char *name, ValueT default_value);
};



#endif