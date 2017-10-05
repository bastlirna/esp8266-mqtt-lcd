#ifndef __FRAME_RENDERER_H
#define __FRAME_RENDERER_H

#include "complex_renderer.h"

class FrameRenderer : public ComplexRenderer{
public:
    FrameRenderer(U8G2_ST7565_64128N_F_4W_HW_SPI& _lcd, JsonObject& _input, JsonObject& _output) : ComplexRenderer(_lcd, _input, _output) {};
    virtual bool draw();
};


#endif