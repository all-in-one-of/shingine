#pragma once
#include "ComponentSetup.h"

class CMaterialComponent : public CComponent, public IAttributeSerialized
{
public:
    ATTRIBUTE_DECL_INIT(CMaterialComponent)
    CMaterialComponent()
    {
        ATTRIBUTE_REGISTER(CMaterialComponent, DiffuseColor)
    };
    virtual ~CMaterialComponent();
    REGISTER_COMPONENT_TYPE(CMaterialComponent)
private:
    ATTRIBUTE_VECTOR_INFO(float, DiffuseColor)
};
