#include "Modules/Graphics/OpenGL/OglTextureManager.h"
#include "Engine/AssetTypes/Texture2D.h"
#include "Modules/Statics/IAssetManager.h"

#include "Utility/Graphics.h"

void OglTextureManager::DeleteUnusedResources() {
  IAssetManager *assetManager = Statics::Get<IAssetManager>();
  AssetTextureMapType::iterator textureMapIterator;
  std::vector<unsigned int> texturesToDelete;
  for (textureMapIterator = AssetIdToTextureId.begin();
       textureMapIterator != AssetIdToTextureId.end(); textureMapIterator++) {
    unsigned int assetId = textureMapIterator->first;
    Texture2D *tex = assetManager->GetAssetOfType<Texture2D>(assetId);
    if (tex)
      continue;
    GLuint textureToDelete = textureMapIterator->second;
    glDeleteTextures(1, &textureToDelete); // & means 'pointer to'
    texturesToDelete.push_back(assetId);
  }

  for (unsigned int x = 0; x < texturesToDelete.size(); x++) {
    textureMapIterator = AssetIdToTextureId.find(texturesToDelete[x]);
    if (textureMapIterator != AssetIdToTextureId.end())
      AssetIdToTextureId.erase(textureMapIterator);
  }
}

unsigned int
OglTextureManager::GetTextureIdByAssetId(unsigned int textureAssetId) {
  AssetTextureMapType::iterator it = AssetIdToTextureId.find(textureAssetId);
  if (it != AssetIdToTextureId.end())
    return it->second;

  unsigned int textureId;
  LoadTextureToGpu(textureAssetId, textureId);
  return textureId;
}

void OglTextureManager::LoadTextureToGpu(unsigned int textureAssetId,
                                         unsigned int &texture) {
  int width, height;
  Texture2D *textureAsset =
      Statics::Get<IAssetManager>()->GetAssetOfType<Texture2D>(textureAssetId);
  textureAsset->GetResolution(width, height);

  // float *data = textureAsset->GetPixels();
  S_LOG_FUNC("Loaded texture #%d", textureAsset->UniqueID());

  // get a texture asset
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  Texture2D::TextureFormat format = textureAsset->GetTextureFormat();
  if (format == Texture2D::TextureFormat::Float_RGBA)
  {
    float *data = textureAsset->GetPixels();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT,
                data);
  }
  
  if (format == Texture2D::TextureFormat::Byte_RGBA)
  {
    unsigned char *data32 = textureAsset->GetPixels32();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                data32);
  }

  glGenerateMipmap(GL_TEXTURE_2D);

  AssetIdToTextureId[textureAssetId] = texture;
}

void OglTextureManager::GetTextureSlotForShaderProgramByName(
    const std::string &name, int programId, int &textureSlot, bool &isNew) {
  isNew = false;
  // first find program id
  TextureSlotMapType::iterator it = TextureSlots.find(programId);
  if (it == TextureSlots.end()) {
    isNew = true;
    textureSlot = 0;
    TextureSlots[programId][name] = textureSlot;
    return;
  }

  NameSlotMapType &programTextureSlotMap = it->second;
  NameSlotMapType::iterator it2 = programTextureSlotMap.find(name);

  if (it2 == programTextureSlotMap.end()) {
    isNew = true;
    textureSlot = programTextureSlotMap.size();
    programTextureSlotMap[name] = textureSlot;
    return;
  }
  textureSlot = it2->second;
}
