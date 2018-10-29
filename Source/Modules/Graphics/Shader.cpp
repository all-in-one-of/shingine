#include "Modules/Graphics/Shader.h"
#include "Modules/Statics/AssetManager.h"

REGISTER_SERIALIZED_NAME(ShaderSource)
REGISTER_SERIALIZED_NAME(Shader)

void Shader::AddSource(EShaderType type, const String &source)
{
    String garbage;
    if (GetSource(type, garbage)) return;
    ShaderSource* shaderSource = 
        AssetManager::Get()->AddAssetOfType<ShaderSource>();
        
    shaderSource->Type = type;
    shaderSource->Source = source;
    Source.push_back(shaderSource);
}

bool Shader::GetSource(EShaderType type, String &source)
{
    for (size_t x = 0; x < Source.size(); x++)
    {
        if (Source[x]->Type == (unsigned int) type)
        {
            source = Source[x]->Source;
            return true;
        }
    }
    return false;
}

unsigned int Shader::AssetId()
{
    return UniqueID();
}
