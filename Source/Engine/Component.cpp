#include "Engine/Components/Component.h"

Component::~Component()
{
}

void Component::SetEntityId(unsigned int entityId)
{ 
    ComponentEntityId = entityId; 
}
