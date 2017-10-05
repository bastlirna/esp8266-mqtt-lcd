#include "base_renderer.h"

template u8g2_uint_t BaseRenderer::loadProperty<u8g2_uint_t>(const char *, u8g2_uint_t); 

template <typename ValueT> ValueT BaseRenderer::loadProperty(const char *name, ValueT default_value)
{
    if(!inputJson.containsKey(name))
    {
        return default_value;
    }
    return inputJson.get<ValueT>(name);
}

