#pragma once
#include "CComponent.h"
#include "SComponentFactory.h"

class CTransformComponent : public CComponent
{
public:
    CTransformComponent();
    virtual ~CTransformComponent();

    REGISTER_COMPONENT_TYPE(CTransformComponent);
    // virtual CString GetTypeName();
    // static const CString TypeName;
    // static SComponentRegistry<CTransformComponent> reg;
};
