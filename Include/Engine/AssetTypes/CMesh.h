#pragma once
#include "Utility/Data/Serialization.h"

class CMesh : public ISerializedClass
{
public:
    SERIALIZE_CLASS(CMesh)
    CMesh()
    {
        ATTRIBUTE_REGISTER(CMesh, Name)
        ATTRIBUTE_REGISTER(CMesh, Indices)
        ATTRIBUTE_REGISTER(CMesh, Normals)
        ATTRIBUTE_REGISTER(CMesh, Positions)
        ATTRIBUTE_REGISTER(CMesh, TexCoord)
    };
    virtual ~CMesh() {}
private:
    ATTRIBUTE_VALUE_INFO(CString, Name)
    ATTRIBUTE_VECTOR_INFO(unsigned int, Indices)
    ATTRIBUTE_VECTOR_INFO(float, Normals)
    ATTRIBUTE_VECTOR_INFO(float, Positions)
    ATTRIBUTE_VECTOR_INFO(float, TexCoord)
};
