#ifndef __LINE_RENDERER_H
#define __LINE_RENDERER_H

#include "complex_renderer.h"

class LineRenderer : public ComplexRenderer{
public:
    LineRenderer(U8G2_ST7565_64128N_F_4W_HW_SPI& _lcd, JsonObject& _input, JsonObject& _output) : ComplexRenderer(_lcd, _input, _output) {};
    virtual bool draw();
};


#endif