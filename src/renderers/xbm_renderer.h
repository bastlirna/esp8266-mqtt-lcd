#ifndef __XBM_RENDERER_H
#define __XBM_RENDERER_H

#include "simple_renderer.h"

class XbmRenderer : public SimpleRenderer{
public:
    XbmRenderer(U8G2_ST7565_64128N_F_4W_HW_SPI& _lcd, JsonObject& _input, JsonObject& _output) : SimpleRenderer(_lcd, _input, _output) {};
    virtual bool draw();
protected:
    u8g2_uint_t w = 0;
    u8g2_uint_t h = 0;

    uint8_t *decodeBitmap(String &value, u8g2_uint_t w, u8g2_uint_t h);
    uint8_t nibble(uint8_t ch);

};


#endif