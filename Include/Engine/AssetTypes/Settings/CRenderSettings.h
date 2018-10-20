#pragma once
#include "Utility/Data/Serialization.h"

class CRenderSettings : public ISerializedClass
{
public:
    SERIALIZE_CLASS(CRenderSettings)
    CRenderSettings()
    {
        ATTRIBUTE_REGISTER(CRenderSettings, ScreenWidth)
        ATTRIBUTE_REGISTER(CRenderSettings, ScreenHeight)
        ATTRIBUTE_REGISTER(CRenderSettings, WindowTitle)
        // default settings
        ScreenWidth = 1280;
        ScreenHeight = 720;
        WindowTitle = "shingine";
    }
    ATTRIBUTE_VALUE(unsigned short, ScreenWidth)
    ATTRIBUTE_VALUE(unsigned short, ScreenHeight)
    ATTRIBUTE_VALUE(CString, WindowTitle)
};
