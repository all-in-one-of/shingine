#include "Modules/Graphics/Shader.h"
#include "Modules/Statics/IAssetManager.h"

REGISTER_SERIALIZED_CLASS(ShaderSource)
REGISTER_SERIALIZED_CLASS(Shader)

void Shader::AddSource(EShaderType type, const String &source) {
  String garbage;
  if (GetSource(type, garbage))
    return;
  ShaderSource *shaderSource =
      Statics::Get<IAssetManager>()->AddAssetOfType<ShaderSource>();

  shaderSource->Type = type;
  shaderSource->Source = source;
  Source.push_back(shaderSource);
}

bool Shader::GetSource(EShaderType type, String &source) {
  for (size_t x = 0; x < Source.size(); x++) {
    if (Source[x]->Type == (unsigned int)type) {
      source = Source[x]->Source;
      return true;
    }
  }
  return false;
}

unsigned int Shader::AssetId() { return UniqueID(); }
