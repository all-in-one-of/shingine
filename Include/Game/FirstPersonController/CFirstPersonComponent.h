#pragma once
#include "Engine/Components/ComponentSetup.h"
#include <glm/glm.hpp>

namespace FirstPersonController
{
    
    class CViewSettings : public ISerializedClass
    {
    public:
        SERIALIZE_CLASS(CViewSettings)
        CViewSettings()
        {
            ATTRIBUTE_REGISTER(CViewSettings, IsSmoothed)
            ATTRIBUTE_REGISTER(CViewSettings, SmoothTime)
            ATTRIBUTE_REGISTER(CViewSettings, SensitivityX)
            ATTRIBUTE_REGISTER(CViewSettings, SensitivityY)
            ATTRIBUTE_REGISTER(CViewSettings, MinX)
            ATTRIBUTE_REGISTER(CViewSettings, MaxX)
            
            IsSmoothed = false;
            SmoothTime = 5.f;
            SensitivityX = .05f;
            SensitivityY = .05f;
            MinX = -90.f;
            MaxX = 90.f;
        };
        virtual ~CViewSettings(){};
        
        ATTRIBUTE_VALUE(unsigned char, IsSmoothed)
        ATTRIBUTE_VALUE(float, SmoothTime)
        ATTRIBUTE_VALUE(float, SensitivityX)
        ATTRIBUTE_VALUE(float, SensitivityY)
        ATTRIBUTE_VALUE(float, MinX)
        ATTRIBUTE_VALUE(float, MaxX)
    };
    
    class CMovementSettings : public ISerializedClass
    {
    public:
        SERIALIZE_CLASS(CMovementSettings)
        CMovementSettings()
        {
            ATTRIBUTE_REGISTER(CMovementSettings, ForwardSpeed)
            ATTRIBUTE_REGISTER(CMovementSettings, BackwardSpeed)
            ATTRIBUTE_REGISTER(CMovementSettings, StrafeSpeed)
            ATTRIBUTE_REGISTER(CMovementSettings, RunMultiplier)
            ATTRIBUTE_REGISTER(CMovementSettings, JumpForce)
            // default values
            ForwardSpeed = 8.0f;
            BackwardSpeed = 4.0f;
            StrafeSpeed = 4.0f;
            RunMultiplier = 2.0f;
        }
        virtual ~CMovementSettings(){};
        ATTRIBUTE_VALUE(float, ForwardSpeed)
        ATTRIBUTE_VALUE(float, BackwardSpeed)
        ATTRIBUTE_VALUE(float, StrafeSpeed)
        ATTRIBUTE_VALUE(float, RunMultiplier)
        ATTRIBUTE_VALUE(float, JumpForce)
    };
    
    class CFirstPersonComponent : public CComponent
    {
    public:
        SERIALIZE_CLASS(CFirstPersonComponent)
        CFirstPersonComponent()
        {
            ATTRIBUTE_REGISTER(CFirstPersonComponent, ViewSettings)
            ATTRIBUTE_REGISTER(CFirstPersonComponent, MovementSettings)
            
            Front = glm::vec3(0);
            Horizontal = glm::vec3(0);
            CameraUp = glm::vec3(0,1,0);
            CameraFront = glm::vec3(0,0,1);
        };
        virtual ~CFirstPersonComponent();
        ATTRIBUTE_CLASS(CViewSettings, ViewSettings)
        ATTRIBUTE_CLASS(CMovementSettings, MovementSettings)

        float Yaw = 0.f;
        float Pitch = 0.f;

        glm::vec3 Front;
        glm::vec3 Horizontal;
        glm::vec3 CameraUp;
        glm::vec3 CameraFront;
    };
};
