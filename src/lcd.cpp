#include <Arduino.h>
#include <Homie.h>
#include <U8g2lib.h>

#include "lcd.h"
#include "keywords.h"
#include "lcd_fonts.h"


#ifndef DEFAULT_FONT
    // https://github.com/olikraus/u8g2/wiki/fntlistmono
    #define DEFAULT_FONT ((uint8_t*)u8g2_font_4x6_mf)
#endif

void LcdClass::begin()
{
    pinMode(backlight_pin, OUTPUT); //BL_PWM
    digitalWrite(backlight_pin, HIGH);// BL_PWM_ON

    lcd.begin();
    lcd.setContrast(150);   
    lcd.clearBuffer();
    
}

bool LcdClass::display(JsonObject& input, JsonObject& output)
{
    bool swapBuffers = true;
    bool success = true;

    if(input.containsKey(KEYWORD_SWAP_BUFFER) && !input.get<bool>(KEYWORD_SWAP_BUFFER)) 
    {
        swapBuffers = false;
    }

    Homie.getLogger() << "display json" << endl;

    if(input.containsKey(KEYWORD_ITEMS)){
        const JsonArray &items = input[KEYWORD_ITEMS];

        JsonArray::const_iterator it = items.begin();
        JsonArray::const_iterator end = items.end();

        while(end != it)
        {
            success &= drawItem(*it, output);
            if(!success){
                break;
            }
            ++it;
        }
    }else{
        Homie.getLogger() << "no items to draw" << endl;
    }

    if(success){
        Homie.getLogger() << "draw success" << endl;
    
        if(swapBuffers)
        {
            Homie.getLogger() << "send buffer" << endl;
            lcd.sendBuffer();
        }
    }

    return success;
}

bool LcdClass::drawItem(JsonObject& item, JsonObject& output)
{
    Homie.getLogger() << "draw item" << endl;
    
    if(item.containsKey(KEYWORD_ITEM_TYPE))
    {
        String type = item[KEYWORD_ITEM_TYPE];

        if(strcasecmp(type.c_str(), KEYWORD_NODE_TYPE_CLEAR) == 0)
        {
            lcd.clearBuffer();
        }
        else if(strcasecmp(type.c_str(), KEYWORD_NODE_TYPE_TEXT) == 0)
        {
            return drawText(item, output);
        }
        else if(strcasecmp(type.c_str(), KEYWORD_NODE_TYPE_LINE) == 0)
        {
            return drawLine(item, output);
        }
        else if(strcasecmp(type.c_str(), KEYWORD_NODE_TYPE_FRAME) == 0)
        {
            return drawFrame(item, output);
        }
        else if(strcasecmp(type.c_str(), KEYWORD_NODE_TYPE_BOX) == 0)
        {
            return drawBox(item, output);
        }
        else
        {
            output[KEYWORD_ERROR] = KEYWORD_ERROR_UNKNOWN_ITEM_TYPE;
            output[KEYWORD_ITEM_TYPE] = type;
            return false;    
        }

    }else{
        output[KEYWORD_ERROR] = KEYWORD_ERROR_MISSING_ITEM_TYPE;
        return false;
    }
    

    return true;
}

template <typename ValueT> ValueT LcdClass::getValue(JsonObject& json, const char *name, ValueT default_value)
{
    if(!json.containsKey(name))
    {
        return default_value;
    }
    return json.get<ValueT>(name);
}

bool LcdClass::drawText(JsonObject& json, JsonObject& output)
{
    Homie.getLogger() << "draw text" << endl;

    uint8_t* font = DEFAULT_FONT; // https://github.com/olikraus/u8g2/wiki/fntlistmono
    if(json.containsKey(KEYWORD_NODE_TEXT_FONT))
    {
        font = FONT_TABLE_find(json.get<const char*>(KEYWORD_NODE_TEXT_FONT));
        if(font == NULL){
            font = DEFAULT_FONT;
            Homie.getLogger() << "font not found" << endl;
            output[KEYWORD_WARNING] = KEYWORD_WARNING_FONT_NOT_FOUND;
        }
    }
    
    lcd.setFont((const uint8_t *)font);

    if(!json.containsKey(KEYWORD_NODE_TEXT_VALUE))
    {
        output[KEYWORD_ERROR] = KEYWORD_ERROR_VALUE_IS_EMPTY;
        return false;
    }

    u8g2_uint_t x = getValue<u8g2_uint_t>(json, KEYWORD_NODE_XPOS, 0);
    u8g2_uint_t y = getValue<u8g2_uint_t>(json, KEYWORD_NODE_YPOS, 0);

    lcd.drawStr(x, y, json.get<const char*>(KEYWORD_NODE_TEXT_VALUE));

    return true;
}

bool LcdClass::drawLine(JsonObject& json, JsonObject& output)
{
    u8g2_uint_t x1 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_X1POS, 0);
    u8g2_uint_t y1 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_Y1POS, 0);

    u8g2_uint_t x2 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_X2POS, 0);
    u8g2_uint_t y2 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_Y2POS, 0);

    lcd.drawLine(x1, y1, x2, y2);

    return true;
}

bool LcdClass::drawFrame(JsonObject& json, JsonObject& output)
{
    u8g2_uint_t x1 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_X1POS, 0);
    u8g2_uint_t y1 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_Y1POS, 0);

    u8g2_uint_t x2 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_X2POS, 0);
    u8g2_uint_t y2 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_Y2POS, 0);

    lcd.drawFrame(x1, y1, x2, y2);

    return true;
}

bool LcdClass::drawBox(JsonObject& json, JsonObject& output)
{
    u8g2_uint_t x1 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_X1POS, 0);
    u8g2_uint_t y1 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_Y1POS, 0);

    u8g2_uint_t x2 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_X2POS, 0);
    u8g2_uint_t y2 = getValue<u8g2_uint_t>(json, KEYWORD_NODE_Y2POS, 0);

    lcd.drawBox(x1, y1, x2, y2);

    return true;
}