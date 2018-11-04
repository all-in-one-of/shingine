#pragma once
#include "Core.h"
#include "IAssetManager.h"
#include "Utility/Data/Serialization.h"
#include <unordered_map>

class AssetManager : public IAssetManager, public IObject {
public:
  SERIALIZE_CLASS(AssetManager);
  AssetManager() {}

  virtual ~AssetManager() {}
  virtual void RemoveAssetType(String assetType);
  virtual IObject *GetAssetByFileName(const String &fileName);
  virtual IObject *GetAssetOfType(const String &typeName,
                                           unsigned int assetId = 0);
  virtual IObject *AddAssetOfType(const String &typeName);
  virtual bool GetAssetIteratorOfType(const String &typeName,
                                      StringMap::iterator &iterator);
  virtual void AddInstance(IObject *newAsset);
  virtual void SaveExternalAssetPath(const std::string &path,
                                     unsigned int assetId);
  virtual unsigned int IsExternalAssetLoaded(const String &fileName);
  virtual void UnloadSceneAssets();

private:
  StringMap Assets;
  typedef std::unordered_map<std::string, unsigned int> ExternalPathMap;
  ExternalPathMap ExternalPathToAssetId;
};
