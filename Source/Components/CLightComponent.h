#pragma once
#include "ComponentSetup.h"

class CLightComponent : public CComponent, public IAttributeSerialized
{
public:
    ATTRIBUTE_DECL_INIT(CLightComponent)
    CLightComponent()
    {
        ATTRIBUTE_REGISTER(CLightComponent, Color)
        ATTRIBUTE_REGISTER(CLightComponent, Exposure)
        ATTRIBUTE_REGISTER(CLightComponent, Intensity)
    };
    virtual ~CLightComponent();
    REGISTER_COMPONENT_TYPE(CLightComponent)
private:
    ATTRIBUTE_VECTOR_INFO(float, Color)
    ATTRIBUTE_VALUE_INFO(float, Exposure)
    ATTRIBUTE_VALUE_INFO(float, Intensity)
};
