#pragma once
#include "../../Utility/Data/Serialization.h"

class CMaterial : public ISerializedClass
{
public:
    SERIALIZE_CLASS(CMaterial)
    CMaterial()
    {
        ATTRIBUTE_REGISTER(CMaterial, Name)
        ATTRIBUTE_REGISTER(CMaterial, DiffuseColor)
    };
    virtual ~CMaterial() {};
private:
    ATTRIBUTE_VALUE_INFO(CString, Name)
    ATTRIBUTE_VECTOR_INFO(float, DiffuseColor)
};

