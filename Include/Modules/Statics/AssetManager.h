#pragma once
#include <unordered_map>
#include "Core.h"
#include "Utility/Data/Serialization.h"
#include "IAssetManager.h"

class AssetManager : public IAssetManager, public ISerializedClass
{
public:
    SERIALIZE_CLASS(AssetManager)
    AssetManager() {}

    virtual ~AssetManager() {}
    virtual void RemoveAssetType(String assetType);
    virtual ISerializedClass* GetAssetOfType(const String &typeName, unsigned int assetId = 0);
    virtual ISerializedClass* AddAssetOfType(const String &typeName);
    virtual bool GetAssetIteratorOfType(const String &typeName, StringMap::iterator &iterator);
    virtual void AddInstance(ISerializedClass* newAsset);
private:
    StringMap Assets;
};
