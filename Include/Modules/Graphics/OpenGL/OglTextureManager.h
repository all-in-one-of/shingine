#pragma once
#include <unordered_map>
#include <string>
class OglTextureManager
{
public:
    OglTextureManager(){}
    unsigned int GetTextureIdByAssetId(unsigned int textureAssetId);
    void LoadTextureToGpu(unsigned int textureAssetId, unsigned int &texture);
    void GetTextureSlotForShaderProgramByName(const std::string &name, 
        int programId, int &textureSlot, bool &isNew);
private:
    typedef std::unordered_map<unsigned int, unsigned int> AssetTextureMapType;
    typedef std::unordered_map<std::string, int> NameSlotMapType;
    typedef std::unordered_map<int, NameSlotMapType> TextureSlotMapType;
    AssetTextureMapType AssetIdToTextureId;

    // get texture slot location
    // program id -> name -> texture slot
    TextureSlotMapType TextureSlots;
};
