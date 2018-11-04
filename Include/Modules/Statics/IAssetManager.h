#pragma once
#include "Statics.h"
#include <typeinfo>
#include <unordered_map>

class IAssetManager {
public:
  typedef std::unordered_map<unsigned int, IObject *> IdMap;
  typedef std::unordered_map<std::string, IdMap> StringMap;

  virtual ~IAssetManager() {}
  virtual void RemoveAssetType(String assetType) = 0;
  virtual IObject *GetAssetByFileName(const String &fileName) = 0;
  virtual IObject *GetAssetOfType(const String &typeName,
                                           unsigned int assetId = 0) = 0;
  virtual IObject *AddAssetOfType(const String &typeName) = 0;
  virtual bool GetAssetIteratorOfType(const String &typeName,
                                      StringMap::iterator &iterator) = 0;
  virtual void AddInstance(IObject *newAsset) = 0;
  virtual void SaveExternalAssetPath(const std::string &path,
                                     unsigned int assetId) = 0;
  virtual void UnloadSceneAssets() = 0;
  virtual unsigned int IsExternalAssetLoaded(const String &fileName) = 0;

  template <class T> T *GetAssetOfType(unsigned int assetId = 0) {
    String typeName = typeid(T).name();
    SerializedFactory::GetDemangledName(typeName);
    IObject *asset = GetAssetOfType(typeName, assetId);
    return dynamic_cast<T *>(asset);
  }

  template <class T> T *AddAssetOfType() {
    String typeName = typeid(T).name();
    SerializedFactory::GetDemangledName(typeName);
    IObject *asset = AddAssetOfType(typeName);
    return dynamic_cast<T *>(asset);
  }
};
