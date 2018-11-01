#include "Modules/Statics/AssetManager.h"
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

ISerializedClass *AssetManager::AddAssetOfType(const String &typeName) {
  ISerializedClass *newAsset = dynamic_cast<ISerializedClass *>(
      SerializedFactory::CreateInstance(typeName.GetStdString()));
  AddInstance(newAsset);
  return newAsset;
}

void AssetManager::AddInstance(ISerializedClass *newAsset) {
  Assets[newAsset->SerializedName().GetStdString()][newAsset->UniqueID()] =
      newAsset;
}

void AssetManager::SaveExternalAssetPath(const std::string &path,
                                         unsigned int assetId) {
  ExternalPathToAssetId[path] = assetId;
}

void AssetManager::RemoveAssetType(String assetType) {
  Assets.erase(assetType.GetStdString());
}

bool AssetManager::GetAssetIteratorOfType(const String &typeName,
                                          StringMap::iterator &iterator) {
  StringMap::iterator it = Assets.find(typeName.GetStdString());
  if (it == Assets.end())
    return false;
  iterator = it;
  return true;
}
