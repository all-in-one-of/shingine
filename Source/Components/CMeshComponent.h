#pragma once
#include "ComponentSetup.h"

class CMeshComponent : public CComponent, public ISerializedClass
{
public:
    ATTRIBUTE_DECL_INIT(CMeshComponent)
    CMeshComponent()
    {
        ATTRIBUTE_REGISTER(CMeshComponent, Indices)
        ATTRIBUTE_REGISTER(CMeshComponent, Normals)
        ATTRIBUTE_REGISTER(CMeshComponent, Positions)
        ATTRIBUTE_REGISTER(CMeshComponent, TexCoord)
    };
    virtual ~CMeshComponent();
    REGISTER_SERIALIZED_TYPE(CMeshComponent)

private:
    ATTRIBUTE_VECTOR_INFO(unsigned int, Indices)
    ATTRIBUTE_VECTOR_INFO(float, Normals)
    ATTRIBUTE_VECTOR_INFO(float, Positions)
    ATTRIBUTE_VECTOR_INFO(float, TexCoord)
};
