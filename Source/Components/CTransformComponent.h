#pragma once
#include "CComponent.h"
#include "SComponentFactory.h"

class CTransformComponent : public CComponent
{
public:
    CTransformComponent();
    virtual ~CTransformComponent();
    REGISTER_COMPONENT_TYPE(CTransformComponent);
private:
    // static const CString TypeName;
};
