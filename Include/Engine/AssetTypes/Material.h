#pragma once
#include "Utility/Data/Serialization.h"
#include "IMaterial.h"

class Material : public IMaterial, public ISerializedClass
{
public:
    SERIALIZE_CLASS(Material)
    Material();
    virtual ~Material() {};
    
    ATTRIBUTE_VALUE(String, Name)
    ATTRIBUTE_ID(ShaderId)

    ATTRIBUTE_VECTOR(String, FloatUniformNames)
    ATTRIBUTE_VECTOR(float, FloatUniformValues)

    ATTRIBUTE_VECTOR(String, VectorUniformNames)
    ATTRIBUTE_VECTOR(float, VectorUniformValues)

    virtual void GetFloatUniforms(std::vector<std::string> &names, std::vector<float> &values);
    virtual void GetVectorUniforms(std::vector<std::string> &names, std::vector<glm::vec4> &values);

};

