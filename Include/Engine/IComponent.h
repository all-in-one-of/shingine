#pragma once
#include "Common.h"

class IObject;

class IComponent
{
public:
    virtual ~IComponent() {};
    virtual unsigned int Id() = 0;
    virtual unsigned int EntityId() = 0;
    virtual void SetEntityId(unsigned int entityId) = 0;
};
