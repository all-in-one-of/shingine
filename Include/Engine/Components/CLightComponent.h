#pragma once
#include "ComponentSetup.h"

class CLightComponent : public CComponent, public ISerializedClass
{
public:
    SERIALIZE_CLASS(CLightComponent)
    CLightComponent()
    {
        ATTRIBUTE_REGISTER(CLightComponent, Color)
        ATTRIBUTE_REGISTER(CLightComponent, Exposure)
        ATTRIBUTE_REGISTER(CLightComponent, Intensity)
    };
    virtual ~CLightComponent();
private:
    ATTRIBUTE_VECTOR(float, Color)
    ATTRIBUTE_VALUE(float, Exposure)
    ATTRIBUTE_VALUE(float, Intensity)
};
