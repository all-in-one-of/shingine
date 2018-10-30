#include "Engine/AssetTypes/Material.h"
REGISTER_SERIALIZED_NAME(Material)

Material::Material()
{
    ATTRIBUTE_REGISTER(Material, Name)
    ATTRIBUTE_REGISTER(Material, ShaderId)

    ATTRIBUTE_REGISTER(Material, FloatUniformNames)
    ATTRIBUTE_REGISTER(Material, FloatUniformValues)

    ATTRIBUTE_REGISTER(Material, VectorUniformNames)
    ATTRIBUTE_REGISTER(Material, VectorUniformValues)

    ShaderId = 0;
    Name = "defaultMaterial";
}

void FindUniformIndex(int &index, std::vector<String> &names, const String &name)
{
    for (int x = 0; x < names.size(); x++)
    {
        if (names[x] == name)
        {
            index = x;
            return;
        }
    }
    index = -1;
}

void Material::SetFloat(const String &name, float value)
{
    int index; 
    FindUniformIndex(index, FloatUniformNames, name);
    if (index == -1)
    {
        FloatUniformNames.push_back(name);
        FloatUniformValues.push_back(value);
        return;
    }
    FloatUniformValues[index] = value;
}

void Material::SetVector(const String &name, const glm::vec4 &value)
{
    int index; 
    FindUniformIndex(index, VectorUniformNames, name);
    if (index == -1)
    {
        VectorUniformNames.push_back(name);
        VectorUniformValues.push_back(value.x);
        VectorUniformValues.push_back(value.y);
        VectorUniformValues.push_back(value.z);
        VectorUniformValues.push_back(value.w);
        return;
    }
    index *= 4;
    VectorUniformValues[index + 0] = value.x;
    VectorUniformValues[index + 1] = value.y;
    VectorUniformValues[index + 2] = value.z;
    VectorUniformValues[index + 3] = value.w;
}

void Material::SetTexture(const String &name, unsigned int textureId)
{
    int index; 
    FindUniformIndex(index, TextureUniformNames, name);
    if (index == -1)
    {
        TextureUniformNames.push_back(name);
        TextureUniformValues.push_back(textureId);
        return;
    }
    TextureUniformValues[index] = textureId;
}

void Material::GetFloatUniforms(std::vector<std::string> &names, std::vector<float> &values)
{
    names.clear();
    for (size_t x = 0; x < FloatUniformNames.size(); x++)
    {
        names.push_back(FloatUniformNames[x].GetStdString());
        values.push_back(FloatUniformValues[x]);
    }
}

void Material::GetVectorUniforms(std::vector<std::string> &names, std::vector<glm::vec4> &values)
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

void Material::GetTextureUniforms(std::vector<std::string> &names, std::vector<unsigned int> &values)
{
    names.clear();
    for (size_t x = 0; x < TextureUniformNames.size(); x++)
    {
        names.push_back(TextureUniformNames[x].GetStdString());
        values.push_back(TextureUniformValues[x]);
    }
}
