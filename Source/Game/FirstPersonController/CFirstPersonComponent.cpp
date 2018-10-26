#include "Game/FirstPersonController/CFirstPersonComponent.h"
namespace FirstPersonController
{
    REGISTER_SERIALIZED_NAME(CFirstPersonComponent, FirstPersonComponent)
    REGISTER_SERIALIZED_NAME(CViewSettings, FirstPersonView)
    REGISTER_SERIALIZED_NAME(CMovementSettings, FirstPersonMovement)
    
    CFirstPersonComponent::~CFirstPersonComponent()
    {
        delete ViewSettings;
        delete MovementSettings;
    };
};
