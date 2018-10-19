#pragma once
#include <map>
#include "../../Utility/Data/ISerialized.h"
class CAssetManager
{
public:
    static CAssetManager* Get()
    {
        if (!CAssetManager::Instance)
            CAssetManager::Instance = new CAssetManager();
        return CAssetManager::Instance;
    }
    void AddAsset(ISerializedClass* newAsset)
    {
        Assets[newAsset->SerializedName()][newAsset->UniqueID()] = newAsset;
    }
private:
    CAssetManager() {};
    std::map<CString, std::map<unsigned int, ISerializedClass*>> Assets;
    static CAssetManager* Instance;
};
