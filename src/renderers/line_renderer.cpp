#include <Arduino.h>
#include <Homie.h>
#include <U8g2lib.h>

#include "line_renderer.h"
#include "../keywords.h"

bool LineRenderer::draw()
{
    loadCoordinates();
    lcd.drawLine(x1, y1, x2, y2);

    return true;
}