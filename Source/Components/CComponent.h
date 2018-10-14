#pragma once
#include "IComponent.h"

class CComponent : public IComponent
{
public:
    CComponent();
    virtual ~CComponent();
    virtual IObject* Owner();
    virtual void SetOwner(IObject* owner);
protected:
    IObject* ObjectOwner = NULL;
};
