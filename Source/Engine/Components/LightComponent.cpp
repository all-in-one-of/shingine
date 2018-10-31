#include "Engine/Components/LightComponent.h"

REGISTER_SERIALIZED_NAME(LightComponent)

LightComponent::LightComponent()
{
    ATTRIBUTE_REGISTER(LightComponent, Color)
    ATTRIBUTE_REGISTER(LightComponent, Exposure)
    ATTRIBUTE_REGISTER(LightComponent, Intensity)

    Color = {1,1,1};
    Exposure = 1.f;
    Intensity = 1.f;

    LightType = POINT_LIGHT_TYPE;

    Constant = 1.0f; 
    Linear = 0.09f;
    Quadratic = 0.032f;
    
    CutOff = 0.9762959977f;
    ShadowEnabled = 0;
    
    InnerAngle = 15.f;
    OuterAngle = 45.f;
};

LightComponent::~LightComponent()
{
}
