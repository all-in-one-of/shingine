#pragma once
#include "Engine/IComponent.h"
#include "Utility/Data/Serialization.h"

class CComponent : public IComponent, public ISerializedClass
{
public:
    CComponent()
    {
    }
    virtual ~CComponent();
    virtual unsigned int Id() { return UniqueID(); }
    virtual unsigned int EntityId() { return ComponentEntityId; }
    
    virtual void SetEntityId(unsigned int entityId);
protected:
    unsigned int ComponentEntityId = 0;
};
