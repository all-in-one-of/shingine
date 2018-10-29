#pragma once
#include <unordered_map>
#include <typeinfo>
#include "Core.h"
#include "Utility/Data/ISerialized.h"
#include "Utility/Data/SerializedFactory.h"

class AssetManager
{
public:
    typedef std::unordered_map<unsigned int, ISerializedClass*> IdMap;
    typedef std::unordered_map<std::string, IdMap> StringMap;

    static AssetManager* Get() 
    {
        if (!Instance) Instance = new AssetManager();
        return Instance;
    }

    void RemoveAssetType(String assetType);
    ISerializedClass* GetAssetOfType(const String &typeName, unsigned int assetId = 0);
    ISerializedClass* AddAssetOfType(const String &typeName);

    template<class T>
    T* GetAssetOfType(unsigned int assetId = 0)
    {
        String typeName = typeid(T).name();
        SerializedFactory::GetDemangledName(typeName);
        ISerializedClass* asset = GetAssetOfType(typeName, assetId);
        return dynamic_cast<T*>(asset);
    }

    template<class T>
    T* AddAssetOfType()
    {
        String typeName = typeid(T).name();
        SerializedFactory::GetDemangledName(typeName);
        ISerializedClass* asset = AddAssetOfType(typeName);
        return dynamic_cast<T*>(asset);
    }

    bool GetAssetIteratorOfType(const String &typeName, StringMap::iterator &iterator);

    void AddInstance(ISerializedClass* newAsset);
private:
    AssetManager() {}
    static AssetManager* Instance;
    StringMap Assets;
};
