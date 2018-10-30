#include "Modules/Graphics/GraphicsUtils.h"
#include "Modules/Graphics/ICommandBuffer.h"
#include "Modules/Statics/IAssetManager.h"
#include "Engine/AssetTypes/Material.h"
#include <unordered_map>

#include "Core.h"
#include "Modules/Statics/IGraphics.h"
#include "Modules/Graphics/IShader.h"

namespace GraphicsUtils
{
    void SetUniformsFromMaterial(ICommandBuffer* buffer, unsigned int materialId,  unsigned int &shaderId)
    {
        std::unordered_map<std::string, 
            std::unordered_map<unsigned int, ISerializedClass*>>::iterator MaterialIterator;

        Statics::Get<IAssetManager>()->GetAssetIteratorOfType("Material", MaterialIterator);
        Material* material;
        if (materialId == 0)
            materialId = MaterialIterator->second.begin()->first;

        material = dynamic_cast<Material*>(MaterialIterator->second.at(materialId));

        shaderId = material->ShaderId;

        // set material uniforms
        {
            std::vector<std::string> names;
            std::vector<float> floatValues;
            material->GetFloatUniforms(names, floatValues);
            for (size_t x = 0; x < names.size(); x++)
                buffer->SetFloat(names[x], shaderId, floatValues[x]);
            
            std::vector<glm::vec4> vectorValues;
            material->GetVectorUniforms(names, vectorValues);
            for (size_t x = 0; x < names.size(); x++)
                buffer->SetVector(names[x], shaderId, vectorValues[x]);
            // TODO add matrices

            std::vector<unsigned int> textureIds;
            material->GetTextureUniforms(names, textureIds);
            for (size_t x = 0; x < names.size(); x++)
                buffer->SetTexture(names[x], shaderId, textureIds[x]);
        }
    }

    IShader* CreateVertexFragmentShader(const String &vertexFileName, const String &fragmentFileName)
    {
        // set default shader
        IShader *shader = dynamic_cast<IShader *>(
            Statics::Get<IAssetManager>()->AddAssetOfType("Shader"));
        String vertSrc, fragSrc;
        ResourceLoader::LoadText(vertexFileName, vertSrc);
        ResourceLoader::LoadText(fragmentFileName, fragSrc);
        shader->AddSource(EShaderType::VERTEX, vertSrc);
        shader->AddSource(EShaderType::FRAGMENT, fragSrc);
        return shader;
    }
}
