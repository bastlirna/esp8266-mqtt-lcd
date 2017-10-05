#include <Arduino.h>
#include <Homie.h>
#include <U8g2lib.h>

#include "box_renderer.h"
#include "../keywords.h"

bool BoxRenderer::draw()
{
    loadCoordinates();
    lcd.drawBox(x1, y1, x2, y2);

    return true;
}