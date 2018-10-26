#pragma once
#include "Engine/Components/ComponentSetup.h"

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
            SensitivityX = 2.f;
            SensitivityY = 2.f;
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
        };
        virtual ~CFirstPersonComponent();
        ATTRIBUTE_CLASS(CViewSettings, ViewSettings)
        ATTRIBUTE_CLASS(CMovementSettings, MovementSettings)
    };
};
