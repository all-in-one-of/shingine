#pragma once
#include "Core.h"
#include "IAssetManager.h"
#include "Utility/Data/Serialization.h"
#include <unordered_map>

class AssetManager : public IAssetManager, public ISerializedClass {
public:
  SERIALIZE_CLASS(AssetManager);
  AssetManager() {}

  virtual ~AssetManager() {}
  virtual void RemoveAssetType(String assetType);
  virtual ISerializedClass *GetAssetByFileName(const String &fileName);
  virtual ISerializedClass *GetAssetOfType(const String &typeName,
                                           unsigned int assetId = 0);
  virtual ISerializedClass *AddAssetOfType(const String &typeName);
  virtual bool GetAssetIteratorOfType(const String &typeName,
                                      StringMap::iterator &iterator);
  virtual void AddInstance(ISerializedClass *newAsset);
  virtual void SaveExternalAssetPath(const std::string &path,
                                     unsigned int assetId);

private:
  StringMap Assets;
  typedef std::unordered_map<std::string, unsigned int> ExternalPathMap;
  ExternalPathMap ExternalPathToAssetId;
};
