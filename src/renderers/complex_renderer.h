#ifndef __COMPLEX_RENDERER_H
#define __COMPLEX_RENDERER_H

#include "base_renderer.h"

class ComplexRenderer : public BaseRenderer{
public:
    ComplexRenderer(U8G2_ST7565_64128N_F_4W_HW_SPI& _lcd, JsonObject& _input, JsonObject& _output) : BaseRenderer(_lcd, _input, _output) {};
protected:
    u8g2_uint_t x1 = 0;
    u8g2_uint_t y1 = 0;
    u8g2_uint_t x2 = 0;
    u8g2_uint_t y2 = 0;

    bool loadCoordinates();
};


#endif