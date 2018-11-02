#include "Modules/Statics/AssetManager.h"
#include "Engine/AssetTypes/Asset.h"
#include "Utility/Data/ISerialized.h"
#include "Utility/Data/SerializedFactory.h"

REGISTER_SERIALIZED_CLASS(AssetManager)
ISerializedClass *AssetManager::GetAssetOfType(const String &typeName,
                                               unsigned int assetId) {
  StringMap::iterator it;
  if (!GetAssetIteratorOfType(typeName, it))
    return nullptr;

  IdMap &idMap = it->second;
  if (idMap.size() == 0)
    return nullptr;

  IdMap::iterator idIterator = idMap.begin();
  if (assetId == 0)
    return idIterator->second;

  idIterator = idMap.find(assetId);
  if (idIterator == idMap.end())
    return nullptr;

  return idIterator->second;
}

ISerializedClass *AssetManager::GetAssetByFileName(const String &fileName) {
  ExternalPathMap::iterator it = ExternalPathToAssetId.find(fileName);
  if (it == ExternalPathToAssetId.end()) {
    // attempt to load it from file
    Asset *newAsset = nullptr;
    ResourceLoader::LoadAsset(fileName, newAsset);
    return dynamic_cast<ISerializedClass *>(newAsset);
  }
  unsigned int uniqueId = it->second;
  return Statics::FindSerializedObject(uniqueId);
}

ISerializedClass *AssetManager::AddAssetOfType(const String &typeName) {
  ISerializedClass *newAsset = dynamic_cast<ISerializedClass *>(
      SerializedFactory::CreateInstance(typeName));
  AddInstance(newAsset);
  return newAsset;
}

void AssetManager::AddInstance(ISerializedClass *newAsset) {
  Assets[newAsset->SerializedName()][newAsset->UniqueID()] = newAsset;
}

void AssetManager::SaveExternalAssetPath(const std::string &path,
                                         unsigned int assetId) {
  ExternalPathToAssetId[path] = assetId;
}

void AssetManager::RemoveAssetType(String assetType) {
  Assets.erase(assetType);
}

bool AssetManager::GetAssetIteratorOfType(const String &typeName,
                                          StringMap::iterator &iterator) {
  StringMap::iterator it = Assets.find(typeName);
  if (it == Assets.end())
    return false;
  iterator = it;
  return true;
}
