#include "Modules/Statics/AssetManager.h"
#include "Engine/AssetTypes/Asset.h"
#include "Modules/Statics/IGraphics.h"
#include "Utility/Data/ISerialized.h"
#include "Utility/Data/SerializedFactory.h"

#include "Modules/Graphics/IRenderContext.h"

REGISTER_SERIALIZED_CLASS(AssetManager)
IObject *AssetManager::GetAssetOfType(const String &typeName,
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

void AssetManager::UnloadSceneAssets() {
  std::vector<IObject *> assetsToDelete;
  StringMap::iterator it;
  // find scene assets
  for (it = Assets.begin(); it != Assets.end(); it++) {
    IdMap &map = it->second;
    IdMap::iterator idIterator;
    for (idIterator = map.begin(); idIterator != map.end(); idIterator++) {
      IObject *obj = idIterator->second;
      Asset *asset = dynamic_cast<Asset *>(obj);
      if (!asset)
        break;
      // TODO track which objects should deleted created during runtime
      if (asset->GetOrigin() == Asset::OriginType::Scene)
        assetsToDelete.push_back(obj);
    }
  }
  // delete them from asset maps
  for (unsigned int x = 0; x < assetsToDelete.size(); x++) {
    IObject *asset = assetsToDelete[x];
    IdMap &map = Assets.find(asset->SerializedName())->second;
    map.erase(asset->UniqueID());
    Statics::Destroy(asset);
  }
  // Clean loaded assets from gpu
  Statics::Get<IGraphics>()->GetContext()->Cleanup();
}

IObject *AssetManager::GetAssetByFileName(const String &fileName) {
  ExternalPathMap::iterator it = ExternalPathToAssetId.find(fileName);
  if (it == ExternalPathToAssetId.end()) {
    // attempt to load it from file
    Asset *newAsset = nullptr;
    ResourceLoader::LoadAsset(fileName, newAsset);
    IObject *newObjectCasted = dynamic_cast<IObject *>(newAsset);
    return newObjectCasted;
  }
  unsigned int uniqueId = it->second;
  return Statics::FindSerializedObject(uniqueId);
}

IObject *AssetManager::AddAssetOfType(const String &typeName) {
  IObject *newAsset = dynamic_cast<IObject *>(
      SerializedFactory::CreateInstance(typeName));
  AddInstance(newAsset);
  return newAsset;
}

void AssetManager::AddInstance(IObject *newAsset) {
  Assets[newAsset->SerializedName()][newAsset->UniqueID()] = newAsset;
}

void AssetManager::SaveExternalAssetPath(const std::string &path,
                                         unsigned int assetId) {
  ExternalPathToAssetId[path] = assetId;
}

unsigned int AssetManager::IsExternalAssetLoaded(const String &fileName) {
  ExternalPathMap::iterator it = ExternalPathToAssetId.find(fileName);
  return it == ExternalPathToAssetId.end() ? 0 : it->second;
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
