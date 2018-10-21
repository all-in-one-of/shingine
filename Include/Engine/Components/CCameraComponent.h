#pragma once
#include "Utility/Data/Serialization.h"

class CCameraComponent : public ISerializedClass
{
public:
    SERIALIZE_CLASS(CCameraComponent)
    CCameraComponent()
    {
        ATTRIBUTE_REGISTER(CCameraComponent, FOV)
        ATTRIBUTE_REGISTER(CCameraComponent, NearPlane)
        ATTRIBUTE_REGISTER(CCameraComponent, FarPlane)
    }
    ATTRIBUTE_VALUE(float, FOV)
    ATTRIBUTE_VALUE(float, NearPlane)
    ATTRIBUTE_VALUE(float, FarPlane)
};
