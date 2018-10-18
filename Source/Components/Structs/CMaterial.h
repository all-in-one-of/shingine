#pragma once
#include "../Utility/Data/CSerializedFactory.h"
#include "../Utility/Data/CTypedAttribute.h"
#include "../Utility/Data/DSerialized.h"

class CMaterial : public ISerializedClass
{
public:
    ATTRIBUTE_DECL_INIT(CMaterial)
    CMaterial()
    {
        ATTRIBUTE_REGISTER(CMaterial, DiffuseColor)
    };
    virtual ~CMaterial() {};
    REGISTER_SERIALIZED_TYPE(CMaterial)
private:
    ATTRIBUTE_VECTOR_INFO(float, DiffuseColor)
};

REGISTER_SERIALIZED_NAME(CMaterial,Material)