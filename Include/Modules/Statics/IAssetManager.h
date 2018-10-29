#pragma once
#include <unordered_map>
#include <typeinfo>
#include "Statics.h"

class IAssetManager
{
public:
    typedef std::unordered_map<unsigned int, ISerializedClass*> IdMap;
    typedef std::unordered_map<std::string, IdMap> StringMap;

    virtual ~IAssetManager() {}
    virtual void RemoveAssetType(String assetType) = 0;
    virtual ISerializedClass* GetAssetOfType(const String &typeName, unsigned int assetId = 0) = 0;
    virtual ISerializedClass* AddAssetOfType(const String &typeName) = 0;
    virtual bool GetAssetIteratorOfType(const String &typeName, StringMap::iterator &iterator) = 0;
    virtual void AddInstance(ISerializedClass* newAsset) = 0;

    
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
};
