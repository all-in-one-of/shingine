#pragma once
#include "ComponentSetup.h"
#include "Utility/Typedefs.h"

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

        ViewMatrix = glm::mat4(1);
        ProjectionMatrix = glm::mat4(1);
    }

    ATTRIBUTE_VALUE(float, FOV)
    ATTRIBUTE_VALUE(float, NearPlane)
    ATTRIBUTE_VALUE(float, FarPlane)

    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
};
