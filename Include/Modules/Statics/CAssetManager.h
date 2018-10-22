#pragma once
#include <unordered_map>
#include "Core.h"
#include "Utility/Data/ISerialized.h"

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
    ISerializedClass* AddAssetOfType(CString typeName);

    template<class T>
    T* GetAssetOfType(CString typeName, unsigned int assetId = 0)
    {
        ISerializedClass* asset = GetAssetOfType(typeName, assetId);
        return dynamic_cast<T*>(asset);
    }

    template<class T>
    T* AddAssetOfType(CString typeName)
    {
        ISerializedClass* asset = AddAssetOfType(typeName);
        return dynamic_cast<T*>(asset);
    }


    void AddInstance(ISerializedClass* newAsset);
private:
    CAssetManager() {}
    static CAssetManager* Instance;
    StringMap Assets;
};
