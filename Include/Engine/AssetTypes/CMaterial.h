#pragma once
#include "Utility/Data/Serialization.h"
#include "IMaterial.h"

class CMaterial : public IMaterial, public ISerializedClass
{
public:
    SERIALIZE_CLASS(CMaterial)
    CMaterial()
    {
        ATTRIBUTE_REGISTER(CMaterial, Name)
        ATTRIBUTE_REGISTER(CMaterial, ShaderId)

        ATTRIBUTE_REGISTER(CMaterial, FloatUniformNames)
        ATTRIBUTE_REGISTER(CMaterial, FloatUniformValues)

        ATTRIBUTE_REGISTER(CMaterial, VectorUniformNames)
        ATTRIBUTE_REGISTER(CMaterial, VectorUniformValues)
    };
    virtual ~CMaterial() {};
    
    ATTRIBUTE_VALUE(CString, Name)
    ATTRIBUTE_ID(ShaderId)

    ATTRIBUTE_VECTOR(CString, FloatUniformNames)
    ATTRIBUTE_VECTOR(float, FloatUniformValues)

    ATTRIBUTE_VECTOR(CString, VectorUniformNames)
    ATTRIBUTE_VECTOR(float, VectorUniformValues)

    virtual void GetFloatUniforms(std::vector<std::string> &names, std::vector<float> &values);
    virtual void GetVectorUniforms(std::vector<std::string> &names, std::vector<glm::vec4> &values);

};

