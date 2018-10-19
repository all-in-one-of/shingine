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

    ISerializedClass* GetAssetOfType(const CString& assetType)
    {
        std::map<CString, std::map<unsigned int, ISerializedClass*>>::iterator it;
        it = Assets.find(assetType);
        if (it == Assets.end())
            return NULL;
        if (it->second.begin() == it->second.end())
            return NULL;
        return it->second.begin()->second;
    }

private:
    CAssetManager() {};
    std::map<CString, std::map<unsigned int, ISerializedClass*>> Assets;
    static CAssetManager* Instance;
};
