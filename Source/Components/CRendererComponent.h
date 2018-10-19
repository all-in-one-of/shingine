#pragma once
#include "ComponentSetup.h"

class CRendererComponent : public CComponent, public ISerializedClass
{
public:
    ATTRIBUTE_DECL_INIT(CRendererComponent)
    CRendererComponent()
    {
        ATTRIBUTE_REGISTER(CRendererComponent, Enabled)
        ATTRIBUTE_REGISTER(CRendererComponent, DrawType)
        ATTRIBUTE_REGISTER(CRendererComponent, MaterialReference)
        ATTRIBUTE_REGISTER(CRendererComponent, MeshReference)
    }
    virtual ~CRendererComponent() {};
    REGISTER_SERIALIZED_TYPE(CRendererComponent)
private:
    ATTRIBUTE_VALUE_INFO(unsigned char, Enabled)
    ATTRIBUTE_VALUE_INFO(unsigned char, DrawType)
    ATTRIBUTE_ID_REFERENCE(MaterialReference)
    ATTRIBUTE_ID_REFERENCE(MeshReference)
};