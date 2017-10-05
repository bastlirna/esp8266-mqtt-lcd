#ifndef __TEXT_RENDERER_H
#define __TEXT_RENDERER_H

#include "simple_renderer.h"

class TextRenderer : public SimpleRenderer{
public:
    TextRenderer(U8G2_ST7565_64128N_F_4W_HW_SPI& _lcd, JsonObject& _input, JsonObject& _output) : SimpleRenderer(_lcd, _input, _output) {};
    virtual bool draw();


};


#endif