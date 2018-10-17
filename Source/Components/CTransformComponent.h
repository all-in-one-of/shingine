#pragma once
#include "ComponentSetup.h"

class CTransformComponent : public CComponent, public IAttributeSerialized
{
public:
    ATTRIBUTE_DECL_INIT(CTransformComponent)
    CTransformComponent();
    virtual ~CTransformComponent();
    REGISTER_COMPONENT_TYPE(CTransformComponent)
private:
    ATTRIBUTE_VALUE_INFO(unsigned int, ParentID)
    ATTRIBUTE_VECTOR_INFO(float, Matrix)
};
