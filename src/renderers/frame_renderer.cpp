#include <Arduino.h>
#include <Homie.h>
#include <U8g2lib.h>

#include "frame_renderer.h"
#include "../keywords.h"

bool FrameRenderer::draw()
{
    loadCoordinates();
    lcd.drawFrame(x1, y1, x2, y2);

    return true;
}