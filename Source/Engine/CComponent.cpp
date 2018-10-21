#include "Engine/CComponent.h"
#include "Modules/Statics/CInstanceManager.h"

CComponent::~CComponent()
{
}

void CComponent::SetEntityId(unsigned int entityId)
{ 
    ComponentEntityId = entityId; 
    // by default newly created components registered by their unique id in the instancemanager
    // register them by entity id instead
    CInstanceManager* instanceManager = CInstanceManager::Get();
    instanceManager->UpdateComponentEntityId(this);
}