#pragma once
#include "ComponentSetup.h"

class RendererComponent : public Component
{
public:
    SERIALIZE_CLASS(RendererComponent)
    RendererComponent()
    {
        ATTRIBUTE_REGISTER(RendererComponent, Enabled)
        ATTRIBUTE_REGISTER(RendererComponent, DrawType)
        ATTRIBUTE_REGISTER(RendererComponent, MaterialReference)
        ATTRIBUTE_REGISTER(RendererComponent, MeshReference)
    }
    virtual ~RendererComponent() {};

    ATTRIBUTE_VALUE(unsigned char, Enabled)
    ATTRIBUTE_VALUE(unsigned char, DrawType)
    ATTRIBUTE_ID(MaterialReference)
    ATTRIBUTE_ID(MeshReference)
};