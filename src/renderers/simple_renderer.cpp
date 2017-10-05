#include "simple_renderer.h"
#include "../keywords.h"

bool SimpleRenderer::loadCoordinates()
{
    x = loadProperty<u8g2_uint_t>(KEYWORD_NODE_XPOS, 0);
    y = loadProperty<u8g2_uint_t>(KEYWORD_NODE_YPOS, 0);
}
