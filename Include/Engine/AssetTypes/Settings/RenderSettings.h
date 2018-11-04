#pragma once
#include "Utility/Data/Serialization.h"

class RenderSettings : public IObject {
public:
  SERIALIZE_CLASS(RenderSettings)
  RenderSettings() {
    ATTRIBUTE_REGISTER(RenderSettings, ScreenWidth)
    ATTRIBUTE_REGISTER(RenderSettings, ScreenHeight)
    ATTRIBUTE_REGISTER(RenderSettings, WindowTitle)
    // default settings
    ScreenWidth = 1280;
    ScreenHeight = 720;
    WindowTitle = "shingine";
  }
  ATTRIBUTE_VALUE(unsigned short, ScreenWidth)
  ATTRIBUTE_VALUE(unsigned short, ScreenHeight)
  ATTRIBUTE_VALUE(String, WindowTitle)
};
