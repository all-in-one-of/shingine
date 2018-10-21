#pragma once
#include "ComponentSetup.h"

class CRendererComponent : public CComponent
{
public:
    SERIALIZE_CLASS(CRendererComponent)
    CRendererComponent()
    {
        ATTRIBUTE_REGISTER(CRendererComponent, Enabled)
        ATTRIBUTE_REGISTER(CRendererComponent, DrawType)
        ATTRIBUTE_REGISTER(CRendererComponent, MaterialReference)
        ATTRIBUTE_REGISTER(CRendererComponent, MeshReference)
    }
    virtual ~CRendererComponent() {};
private:
    ATTRIBUTE_VALUE(unsigned char, Enabled)
    ATTRIBUTE_VALUE(unsigned char, DrawType)
    ATTRIBUTE_ID(MaterialReference)
    ATTRIBUTE_ID(MeshReference)
};