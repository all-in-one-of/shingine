#include "Modules/Statics/SceneManager.h"
#include <cassert>

#include "Engine/AssetTypes/Asset.h"
#include "Engine/AssetTypes/ExternalAsset.h"
#include "Modules/ResourceLoader/ResourceLoader.h"
#include "Modules/Statics/IAssetManager.h"
#include "Modules/Statics/IEntityManager.h"
#include "Utility/Data/EntityComponentIdSetter.h"
#include "Utility/Data/IDataNode.h"
#include "Utility/Data/UniqueIdSetter.h"

#include "Modules/Statics/IComponentManager.h"
#include "Modules/Statics/IEventSystem.h"

#include <vector>
REGISTER_SERIALIZED_CLASS(SceneManager)
SceneManager::SceneManager(){};

void SceneManager::SetCurrentScene(const String &fileName) {
  CurrentSceneFileName = fileName;
}

String SceneManager::GetCurrentSceneFileName() { return CurrentSceneFileName; }

bool SceneManager::UnloadCurrentScene() {
  if (CurrentSceneFileName == "")
    return true;

  std::vector<unsigned int>::iterator entityIdIterator;
  IEntityManager *entityManager = Statics::Get<IEntityManager>();

  entityManager->DestroyAllEntities();

  IAssetManager *assetManager = Statics::Get<IAssetManager>();
  assetManager->UnloadSceneAssets();

  SetCurrentScene("");
  return true;
}

String GetExternalAssetPathRelativeToTheSceneFile(const String &assetFileName,
                                                  const String &sceneFileName) {
  String sceneRootDir;
  // Get directory of the scene file
  std::vector<String> fileSegments = sceneFileName.Split('/');
  for (unsigned int x = 0; x < fileSegments.size() - 1; x++)
    sceneRootDir += fileSegments[x] + "/";

  return sceneRootDir + assetFileName;
}

bool SceneManager::LoadScene(const String &fileName) {
  if (CurrentSceneFileName == fileName) // this scene is already loaded
    return true;

  // handle scene loading
  assert(UnloadCurrentScene()); // this should do fine

  // unload previous scene
  std::vector<IDataNode *> nodes;
  if (!ResourceLoader::LoadSsd(fileName, nodes))
    return false;
  // traverse through each node, create unique ids
  UniqueIdSetter::SetIds(nodes);

  IAssetManager *assetManager = Statics::Get<IAssetManager>();
  // old id new id pairs
  std::unordered_map<unsigned int, unsigned int> uniqueIdsToReplace;
  for (unsigned int z = 0; z < nodes.size(); z++) {
    IDataNode *node = nodes[z];
    // handle external asset here
    if ((node->Name() == "ExternalAsset") == false)
      continue;
    std::vector<ISerialized *> attributes = node->GetAttributes();
    for (unsigned int x = 0; x < attributes.size(); x++) {
      ISerialized *attr = attributes[x];
      // to do fix the operator
      if ((attr->SerializedName() == "FileName") == false)
        continue;
      TypedAttributeValue<String> *stringAttribute =
          dynamic_cast<TypedAttributeValue<String> *>(attr);
      String assetFileName = stringAttribute->Get();
      unsigned int assetId = assetManager->IsExternalAssetLoaded(
          GetExternalAssetPathRelativeToTheSceneFile(assetFileName, fileName));
      if (assetId == 0)
        continue;
      Statics::ReturnUniqueId(node->GetUniqueID());
      uniqueIdsToReplace[node->GetUniqueID()] = assetId;
    }
  }

  if (uniqueIdsToReplace.size())
    UniqueIdSetter::ReplaceIds(nodes, uniqueIdsToReplace);

  std::vector<ISerialized *> deserializedNodes;
  for (size_t x = 0; x < nodes.size(); x++) {
    ISerialized *deserializedDataNode = nodes[x]->Deserialize();
    if (deserializedDataNode)
      deserializedNodes.push_back(deserializedDataNode);

    // add instance
    IObject *serializedObject =
        dynamic_cast<IObject *>(deserializedDataNode);

    Statics::AddSerializedObject(serializedObject);
  }

  std::vector<ISerialized *> entities;

  for (unsigned int x = 0; x < deserializedNodes.size(); x++) {
    Asset *asset = dynamic_cast<Asset *>(deserializedNodes[x]);
    if (asset) {
      asset->SetOrigin(Asset::OriginType::Scene);
      asset->SetFileName(fileName);
    }

    if (deserializedNodes[x]->SerializedName() == "Entity") {
      entities.push_back(deserializedNodes[x]);
      // won't return the unique id
      delete deserializedNodes[x];
    }

    else if (deserializedNodes[x]->SerializedName() == "EntityIdCollection") {
      EntityComponentIdSetter::UpdateIds(deserializedNodes[x]);
      // will return the unique id
      Statics::Destroy(dynamic_cast<IObject *>(deserializedNodes[x]));
    }

    // handle external node
    else if (deserializedNodes[x]->SerializedName() == "ExternalAsset") {
      LoadExternalAssetFromScene(deserializedNodes[x], fileName);
    }
  }

  // helper types which are supposed to be deleted once the scene is loaded
  assetManager->RemoveAssetType("Entity");
  assetManager->RemoveAssetType("EntityIdCollection");
  assetManager->RemoveAssetType("ExternalAsset");

  for (size_t x = 0; x < nodes.size(); x++)
    delete nodes[x];
    
  Statics::Get<IEventSystem>()->DispatchEvent(EventType::OnSceneLoad);
  SetCurrentScene(fileName);
  return true;
}

void SceneManager::LoadExternalAssetFromScene(ISerialized *obj,
                                              const String &sceneFileName) {
  ExternalAsset *externalAsset = dynamic_cast<ExternalAsset *>(obj);

  const String fileName = GetExternalAssetPathRelativeToTheSceneFile(
      externalAsset->FileName, sceneFileName);

  unsigned int uniqueId = externalAsset->UniqueID();
  delete externalAsset;

  if (Statics::Get<IAssetManager>()->IsExternalAssetLoaded(fileName))
    return;
  Asset *loadedAsset;
  assert(ResourceLoader::LoadAsset(fileName, loadedAsset, uniqueId));
}
