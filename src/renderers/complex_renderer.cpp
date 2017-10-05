#include "complex_renderer.h"

#include "complex_renderer.h"
#include "../keywords.h"

bool ComplexRenderer::loadCoordinates()
{
    x1 = loadProperty<u8g2_uint_t>(KEYWORD_NODE_X1POS, 0);
    y1 = loadProperty<u8g2_uint_t>(KEYWORD_NODE_Y1POS, 0);
    x2 = loadProperty<u8g2_uint_t>(KEYWORD_NODE_X2POS, 0);
    y2 = loadProperty<u8g2_uint_t>(KEYWORD_NODE_Y2POS, 0);
}
