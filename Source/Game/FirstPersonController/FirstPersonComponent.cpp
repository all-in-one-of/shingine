#include "Game/FirstPersonController/FirstPersonComponent.h"
namespace FirstPersonController
{
    REGISTER_SERIALIZED_NAME(FirstPersonComponent)
    REGISTER_SERIALIZED_NAME(ViewSettings)
    REGISTER_SERIALIZED_NAME(MovementSettings)
    
    FirstPersonComponent::~FirstPersonComponent()
    {
        delete PlayerViewSettings;
        delete PlayerMovementSettings;
    };
};
