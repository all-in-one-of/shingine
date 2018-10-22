#pragma once
#include "ISystem.h"

class CSystem : public ISystem
{
public:
    virtual ~CSystem(){};
    virtual void SetActive(bool active);
    virtual bool IsActive();
protected:
    bool Active = true;
};
