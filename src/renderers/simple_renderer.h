#ifndef __SIMPLE_RENDERER_H
#define __SIMPLE_RENDERER_H

#include "base_renderer.h"

class SimpleRenderer : public BaseRenderer{
public:
    SimpleRenderer(U8G2_ST7565_64128N_F_4W_HW_SPI& _lcd, JsonObject& _input, JsonObject& _output) : BaseRenderer(_lcd, _input, _output) {};
protected:
    u8g2_uint_t x = 0;
    u8g2_uint_t y = 0;

    bool loadCoordinates();
};


#endif