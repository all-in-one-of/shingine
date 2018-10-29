#pragma once
#include "ComponentSetup.h"

class LightComponent : public Component
{
public:
    SERIALIZE_CLASS(LightComponent)
    
    LightComponent()
    {
        ATTRIBUTE_REGISTER(LightComponent, Color)
        ATTRIBUTE_REGISTER(LightComponent, Exposure)
        ATTRIBUTE_REGISTER(LightComponent, Intensity)
    };
    virtual ~LightComponent();
private:
    ATTRIBUTE_VECTOR(float, Color)
    ATTRIBUTE_VALUE(float, Exposure)
    ATTRIBUTE_VALUE(float, Intensity)
};
