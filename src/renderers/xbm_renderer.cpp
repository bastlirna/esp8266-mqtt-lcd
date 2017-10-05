#include <Arduino.h>
#include <Homie.h>
#include <U8g2lib.h>
#include <math.h>

#include "xbm_renderer.h"
#include "../keywords.h"
#include "../utils.h"

#ifndef MAX_IMAGE_SIZE
#define MAX_IMAGE_SIZE 2048
#endif

uint8_t XbmRenderer::nibble(uint8_t ch)
{
    if(ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }
    else if(ch >= 'A' && ch <= 'F')
    {
        return ch - 'A' + 10;
    }
    else if(ch >= 'a' && ch <= 'f')
    {
        return ch - 'a' + 10;
    }

    return 0;
}

uint8_t *XbmRenderer::decodeBitmap(String &value, u8g2_uint_t w, u8g2_uint_t h)
{
    const char *inputString = value.c_str();

    uint32_t len = l_min(l_min(strlen(inputString), MAX_IMAGE_SIZE) / 2, ceil((float)h / 8.0) * w); 
    
    if(len == 0)
    {
        return NULL;
    }

    uint8_t *ret = (uint8_t *)malloc(len);
    
    uint8_t *ptr = ret;
    while(len > 0 && ret != NULL)
    {
        *ptr = nibble(*inputString) << 4 | nibble(*(inputString + 1));
        ptr ++;
        len --;
        inputString += 2;
    }
    
    return ret;
}

bool XbmRenderer::draw()
{
    Homie.getLogger() << "draw xbm" << endl;

    if(!inputJson.containsKey(KEYWORD_NODE_DATA))
    {
        outputJson[KEYWORD_ERROR] = KEYWORD_ERROR_DATA_IS_EMPTY;
        return false;
    }

    loadCoordinates();
    w = loadProperty<u8g2_uint_t>(KEYWORD_NODE_WIDTH, 0);
    h = loadProperty<u8g2_uint_t>(KEYWORD_NODE_HEIGHT, 0);

    String data = inputJson.get<String>(KEYWORD_NODE_DATA);
    uint8_t *bitmap = decodeBitmap(data, w, h);

    if(bitmap == NULL)
    {
        outputJson[KEYWORD_ERROR] = KEYWORD_ERROR_BAD_IMAGE_SIZE;
        return false;
    }

    lcd.drawXBM(x, y, w, h, bitmap);
    free(bitmap);

    return true;
    
}