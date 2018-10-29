#pragma once
#include "Engine/IComponent.h"
#include "Utility/Data/Serialization.h"

class Component : public IComponent, public ISerializedClass
{
public:
    Component()
    {
    }
    virtual ~Component();
    virtual unsigned int Id() { return UniqueID(); }
    virtual unsigned int EntityId() { return ComponentEntityId; }
    
    virtual void SetEntityId(unsigned int entityId);
protected:
    unsigned int ComponentEntityId = 0;
};
