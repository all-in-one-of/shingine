#include "Engine/AssetTypes/CMaterial.h"
REGISTER_SERIALIZED_NAME(CMaterial,Material)

void CMaterial::GetFloatUniforms(std::vector<std::string> &names, std::vector<float> &values)
{
    names.clear();
    for (size_t x = 0; x < FloatUniformNames.size(); x++)
    {
        names.push_back(FloatUniformNames[x].GetStdString());
        values.push_back(FloatUniformValues[x]);
    }
}

void CMaterial::GetVectorUniforms(std::vector<std::string> &names, std::vector<glm::vec4> &values)
{
    names.clear();
    for (size_t x = 0; x < VectorUniformNames.size(); x++)
    {
        names.push_back(VectorUniformNames[x].GetStdString());
        size_t y = x * 4; // vec4
        if (y >= VectorUniformValues.size())
            throw 1;
        values.push_back(glm::vec4(VectorUniformValues[y + 0], VectorUniformValues[y + 1],
            VectorUniformValues[y + 2], VectorUniformValues[y + 3]));
    }
}
