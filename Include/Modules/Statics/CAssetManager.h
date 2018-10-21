#pragma once
#include <unordered_map>
#include "Core.h"

class ISerializedClass;
class CAssetManager
{
public:
    typedef std::unordered_map<unsigned int, ISerializedClass*> IdMap;
    typedef std::unordered_map<std::string, IdMap> StringMap;

    static CAssetManager* Get() 
    {
        if (!Instance) Instance = new CAssetManager();
        return Instance;
    }

    void RemoveAssetType(CString assetType);
    ISerializedClass* GetAssetOfType(CString typeName, unsigned int assetId = 0);
    void AddInstance(ISerializedClass* newAsset);
private:
    CAssetManager() {}
    static CAssetManager* Instance;
    StringMap Assets;
};
