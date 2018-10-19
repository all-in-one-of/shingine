#pragma once
#include "ComponentSetup.h"

class CTransformComponent : public CComponent, public ISerializedClass
{
public:
    ATTRIBUTE_DECL_INIT(CTransformComponent)
    CTransformComponent()
    {
        ATTRIBUTE_REGISTER(CTransformComponent, ParentID)
        ATTRIBUTE_REGISTER(CTransformComponent, Matrix)
    }
    virtual ~CTransformComponent() {};
    REGISTER_SERIALIZED_TYPE(CTransformComponent)
private:
    ATTRIBUTE_ID_REFERENCE(ParentID)
    ATTRIBUTE_VECTOR_INFO(float, Matrix)
};
