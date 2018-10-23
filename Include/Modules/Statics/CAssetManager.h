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
    ISerializedClass* GetAssetOfType(const CString &typeName, unsigned int assetId = 0);
    ISerializedClass* AddAssetOfType(const CString &typeName);

    template<class T>
    T* GetAssetOfType(const CString &typeName, unsigned int assetId = 0)
    {
        ISerializedClass* asset = GetAssetOfType(typeName, assetId);
        return dynamic_cast<T*>(asset);
    }

    template<class T>
    T* AddAssetOfType(const CString &typeName)
    {
        ISerializedClass* asset = AddAssetOfType(typeName);
        return dynamic_cast<T*>(asset);
    }

    bool GetAssetIteratorOfType(const CString &typeName, StringMap::iterator &iterator);

    void AddInstance(ISerializedClass* newAsset);
private:
    CAssetManager() {}
    static CAssetManager* Instance;
    StringMap Assets;
};
