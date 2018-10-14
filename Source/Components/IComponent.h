#pragma once
#include "Common.h"

class IObject;

class IComponent
{
public:
    virtual ~IComponent() {};
    virtual CString GetTypeName() = 0;
    virtual IObject* Owner() = 0;
    virtual void SetOwner(IObject* owner) = 0;
};
