#pragma once
#include "ComponentSetup.h"

class CTransformComponent : public CComponent, public ISerializedClass
{
public:
    ATTRIBUTE_DECL_INIT(CTransformComponent)
    CTransformComponent();
    virtual ~CTransformComponent() {};
    REGISTER_SERIALIZED_TYPE(CTransformComponent)
private:
    ATTRIBUTE_VALUE_INFO(unsigned int, ParentID)
    ATTRIBUTE_VECTOR_INFO(float, Matrix)
};
