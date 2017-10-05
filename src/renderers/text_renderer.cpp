#include <Arduino.h>
#include <Homie.h>
#include <U8g2lib.h>

#include "text_renderer.h"
#include "../lcd_fonts.h"
#include "../keywords.h"

#ifndef DEFAULT_FONT
// https://github.com/olikraus/u8g2/wiki/fntlistmono
#define DEFAULT_FONT ((uint8_t*)u8g2_font_4x6_mf)
#endif

bool TextRenderer::draw()
{
    Homie.getLogger() << "draw text" << endl;
    
    uint8_t* font = DEFAULT_FONT; // https://github.com/olikraus/u8g2/wiki/fntlistmono

    if(inputJson.containsKey(KEYWORD_NODE_TEXT_FONT))
    {
        font = FONT_TABLE_find(inputJson.get<const char*>(KEYWORD_NODE_TEXT_FONT));
        if(font == NULL){
            font = DEFAULT_FONT;
            Homie.getLogger() << "font not found" << endl;
            outputJson[KEYWORD_WARNING] = KEYWORD_WARNING_FONT_NOT_FOUND;
        }
    }
    
    lcd.setFont((const uint8_t *)font);

    if(!inputJson.containsKey(KEYWORD_NODE_TEXT_VALUE))
    {
        outputJson[KEYWORD_ERROR] = KEYWORD_ERROR_VALUE_IS_EMPTY;
        return false;
    }

    loadCoordinates();
    
    lcd.drawStr(x, y, inputJson.get<const char*>(KEYWORD_NODE_TEXT_VALUE));

    return true;
    
}