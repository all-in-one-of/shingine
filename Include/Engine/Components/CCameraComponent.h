#pragma once
#include "ComponentSetup.h"

class CCameraComponent : public CComponent
{
public:
    SERIALIZE_CLASS(CCameraComponent)
    CCameraComponent()
    {
        ATTRIBUTE_REGISTER(CCameraComponent, FOV)
        ATTRIBUTE_REGISTER(CCameraComponent, NearPlane)
        ATTRIBUTE_REGISTER(CCameraComponent, FarPlane)

        FOV = .6f;
        NearPlane = .1f;
        FarPlane = 1000.f;
    }
    ATTRIBUTE_VALUE(float, FOV)
    ATTRIBUTE_VALUE(float, NearPlane)
    ATTRIBUTE_VALUE(float, FarPlane)
};
