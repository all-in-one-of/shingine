#pragma once
#include "ComponentSetup.h"
#include "Utility/Typedefs.h"

class CameraComponent : public Component
{
public:
    SERIALIZE_CLASS(CameraComponent)
    CameraComponent()
    {
        ATTRIBUTE_REGISTER(CameraComponent, FOV)
        ATTRIBUTE_REGISTER(CameraComponent, NearPlane)
        ATTRIBUTE_REGISTER(CameraComponent, FarPlane)

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
