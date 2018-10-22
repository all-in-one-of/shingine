#include "Modules/Graphics/CShader.h"
#include "Modules/Statics/CAssetManager.h"

REGISTER_SERIALIZED_NAME(CShaderSource,ShaderSource)
REGISTER_SERIALIZED_NAME(CShader,Shader)

void CShader::AddSource(EShaderType type, const CString &source)
{
    CString garbage;
    if (GetSource(type, garbage)) return;
    CShaderSource* shaderSource = 
        CAssetManager::Get()->AddAssetOfType<CShaderSource>("ShaderSource");
    shaderSource->Type = type;
    shaderSource->Source = source;
    Source.push_back(shaderSource);
}

bool CShader::GetSource(EShaderType type, CString &source)
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
