#pragma once
#include "Common.h"

class IObject;

class IComponent
{
public:
    virtual ~IComponent() {};
    virtual IObject* Owner() = 0;
    virtual void SetOwner(IObject* owner);
};
