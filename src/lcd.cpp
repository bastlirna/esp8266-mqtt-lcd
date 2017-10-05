#include <Arduino.h>
#include <Homie.h>
#include <U8g2lib.h>

#include "utils.h"
#include "lcd.h"
#include "keywords.h"

#include "renderers.h"

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

    if(input.containsKey(KEYWORD_BACKLIGHT))
    {
        float intensity = input.get<float>(KEYWORD_BACKLIGHT);

        Homie.getLogger() << "setting BL to " << intensity << endl;

        intensity *= 1023.0;
        intensity = l_max(l_min(intensity, 1023.0), 0.0);

        analogWrite(backlight_pin, 1023 - (uint16_t)intensity);
    }   

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

        BaseRenderer* renderer = NULL;

        if(strcasecmp(type.c_str(), KEYWORD_NODE_TYPE_CLEAR) == 0)
        {
            lcd.clearBuffer();
        }
        else if(strcasecmp(type.c_str(), KEYWORD_NODE_TYPE_TEXT) == 0)
        {
            renderer = new TextRenderer(lcd, item, output);
        }
        else if(strcasecmp(type.c_str(), KEYWORD_NODE_TYPE_LINE) == 0)
        {
            renderer = new LineRenderer(lcd, item, output);
        }
        else if(strcasecmp(type.c_str(), KEYWORD_NODE_TYPE_FRAME) == 0)
        {
            renderer = new FrameRenderer(lcd, item, output);
        }
        else if(strcasecmp(type.c_str(), KEYWORD_NODE_TYPE_BOX) == 0)
        {
            renderer = new BoxRenderer(lcd, item, output);
        }
        else if(strcasecmp(type.c_str(), KEYWORD_NODE_TYPE_XBM) == 0)
        {
            renderer = new XbmRenderer(lcd, item, output);
        }
        else
        {
            output[KEYWORD_ERROR] = KEYWORD_ERROR_UNKNOWN_ITEM_TYPE;
            output[KEYWORD_ITEM_TYPE] = type;
            return false;    
        }

        if(renderer != NULL)
        {
            bool ret = renderer->draw();
            delete renderer;
            return ret;
        }

    }else{
        output[KEYWORD_ERROR] = KEYWORD_ERROR_MISSING_ITEM_TYPE;
        return false;
    }
    

    return true;
}

