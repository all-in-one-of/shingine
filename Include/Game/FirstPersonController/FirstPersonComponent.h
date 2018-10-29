#pragma once
#include "Engine/Components/ComponentSetup.h"
#include <glm/glm.hpp>

namespace FirstPersonController
{
    class ViewSettings : public ISerializedClass
    {
    public:
        SERIALIZE_CLASS(ViewSettings)
        ViewSettings()
        {
            ATTRIBUTE_REGISTER(ViewSettings, IsSmoothed)
            ATTRIBUTE_REGISTER(ViewSettings, SmoothTime)
            ATTRIBUTE_REGISTER(ViewSettings, SensitivityX)
            ATTRIBUTE_REGISTER(ViewSettings, SensitivityY)
            ATTRIBUTE_REGISTER(ViewSettings, MinX)
            ATTRIBUTE_REGISTER(ViewSettings, MaxX)
            
            IsSmoothed = false;
            SmoothTime = 5.f;
            SensitivityX = .05f;
            SensitivityY = .05f;
            MinX = -90.f;
            MaxX = 90.f;
        };
        virtual ~ViewSettings(){};
        
        ATTRIBUTE_VALUE(unsigned char, IsSmoothed)
        ATTRIBUTE_VALUE(float, SmoothTime)
        ATTRIBUTE_VALUE(float, SensitivityX)
        ATTRIBUTE_VALUE(float, SensitivityY)
        ATTRIBUTE_VALUE(float, MinX)
        ATTRIBUTE_VALUE(float, MaxX)
    };
    
    class MovementSettings : public ISerializedClass
    {
    public:
        SERIALIZE_CLASS(MovementSettings)
        MovementSettings()
        {
            ATTRIBUTE_REGISTER(MovementSettings, ForwardSpeed)
            ATTRIBUTE_REGISTER(MovementSettings, BackwardSpeed)
            ATTRIBUTE_REGISTER(MovementSettings, StrafeSpeed)
            ATTRIBUTE_REGISTER(MovementSettings, RunMultiplier)
            ATTRIBUTE_REGISTER(MovementSettings, JumpForce)
            // default values
            ForwardSpeed = 8.0f;
            BackwardSpeed = 4.0f;
            StrafeSpeed = 4.0f;
            RunMultiplier = 2.0f;
        }
        virtual ~MovementSettings(){};
        ATTRIBUTE_VALUE(float, ForwardSpeed)
        ATTRIBUTE_VALUE(float, BackwardSpeed)
        ATTRIBUTE_VALUE(float, StrafeSpeed)
        ATTRIBUTE_VALUE(float, RunMultiplier)
        ATTRIBUTE_VALUE(float, JumpForce)
    };
    
    class FirstPersonComponent : public Component
    {
    public:
        SERIALIZE_CLASS(FirstPersonComponent)
        FirstPersonComponent()
        {
            ATTRIBUTE_REGISTER(FirstPersonComponent, PlayerViewSettings)
            ATTRIBUTE_REGISTER(FirstPersonComponent, PlayerMovementSettings)
            
            Front = glm::vec3(0);
            Horizontal = glm::vec3(0);
            CameraUp = glm::vec3(0,1,0);
            CameraFront = glm::vec3(0,0,1);
        };
        virtual ~FirstPersonComponent();
        ATTRIBUTE_CLASS(ViewSettings, PlayerViewSettings)
        ATTRIBUTE_CLASS(MovementSettings, PlayerMovementSettings)

        float Yaw = 0.f;
        float Pitch = 0.f;

        glm::vec3 Front;
        glm::vec3 Horizontal;
        glm::vec3 CameraUp;
        glm::vec3 CameraFront;
    };
};
