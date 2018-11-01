#include <fstream>
#include <iostream>
#include <stdio.h>

#include "Modules/ResourceLoader/CResourceReaderFactory.h"
#include "Modules/ResourceLoader/ResourceLoader.h"

#include "Modules/Scene/SceneMaker.h"
#include "Utility/Data/EntityComponentIdSetter.h"
#include "Utility/Data/IDataNode.h"
#include "Utility/Data/ISerialized.h"
#include "Utility/Data/UniqueIdSetter.h"

#include "Modules/Statics/IAssetManager.h"
#include "Modules/Statics/ISceneManager.h"

#include "Engine/AssetTypes/Asset.h"
#include "Engine/AssetTypes/ExternalAsset.h"
#include "Modules/ResourceLoader/BitmapReader/BitmapReader.h"

String ResourceLoader::LastError = "";
String ResourceLoader::GetLastError() { return LastError; }

void ResourceLoader::SetupPath(const String &localPath, String &outPath) {
  const String basePath = "/Users/snowbear/Documents/GIT/shingine/";
  outPath = basePath + localPath;
}

bool ResourceLoader::LoadBitmap(const String &fileName,
                                ISerializedClass *&textureAsset) {
  String updatedFileName;
  SetupPath(fileName, updatedFileName);
  textureAsset = Statics::Get<IAssetManager>()->AddAssetOfType("Texture2D");
  return BitmapReader::ReadBitmapToTexture(updatedFileName.GetCharArray(),
                                           textureAsset);
}

bool ResourceLoader::LoadText(const String &fileName, String &data) {
  String updatedFileName;
  SetupPath(fileName, updatedFileName);

  FILE *file;
#if defined _WIN32 || defined _WIN64
  errno_t err;
  if ((err = fopen_s(&file, updatedFileName.GetCharArray(), "rb")) != 0)
#else
  file = fopen(updatedFileName.GetCharArray(), "rb");
  if (!file)
#endif
  {
    LastError = "Couldn't open file : " + fileName;
    return false;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *str = new char[fileSize + 1];
  fread(str, fileSize, 1, file);
  fclose(file);

  str[fileSize] = '\0';
  data = String(str);

  delete[] str;
  return true;
}

void ResourceLoader::LoadExternalAsset(ISerialized *obj,
                                       const String &sceneFileName) {
  ExternalAsset *externalAsset = dynamic_cast<ExternalAsset *>(obj);
  std::vector<IDataNode *> nodes;

  String sceneRootDir;
  // Get directory of the scene file
  std::vector<String> fileSegments = sceneFileName.Split('/');
  for (unsigned int x = 0; x < fileSegments.size() - 1; x++)
    sceneRootDir += fileSegments[x] + "/";

  const String fileName = externalAsset->FileName;
  const String fileNameFull = sceneRootDir + fileName;
  std::cout << fileNameFull.GetStdString() << std::endl;

  bool ssdLoaded = LoadSsd(fileNameFull, nodes);

  assert(ssdLoaded);
  assert(nodes.size() == 1); // the node count should be one

  unsigned int uniqueId = externalAsset->UniqueID();
  delete externalAsset;

  ISerialized *deserializedObj = nodes[0]->Deserialize();
  ISerializedClass *serializedClass =
      dynamic_cast<ISerializedClass *>(deserializedObj);
  serializedClass->SetUniqueID(uniqueId);

  Statics::AddSerializedObject(serializedClass);
  Asset *asset = dynamic_cast<Asset *>(serializedClass);
  assert(asset != NULL);
  // set run time parameters
  asset->SetOrigin(Asset::OriginType::External);
  asset->SetFileName(fileName);
}

bool ResourceLoader::LoadSsd(const String &fileName,
                             std::vector<IDataNode *> &nodes) {
  // 3 file types are supported
  // *.ssd - binary version
  // *.ssda - ascii version
  // *.ssd_json - json

  String updatedFileName;
  SetupPath(fileName, updatedFileName);

  CResourceReaderFactory ResourceReaderFactory;
  IResourceReader *reader = ResourceReaderFactory.CreateReader(updatedFileName);

  if (!reader->Open()) {
    LastError = "Couldn't open file : " + fileName;
    delete reader;
    return false;
  }

  reader->ReadNodes(nodes);
  reader->Close();

  delete reader;
  return true;
}

bool ResourceLoader::Load(const String &fileName) {

  std::vector<IDataNode *> nodes;
  if (!LoadSsd(fileName, nodes))
    return false;
  // traverse through each node, create unique ids
  UniqueIdSetter::SetIds(nodes);

  std::vector<ISerialized *> deserializedNodes;
  for (size_t x = 0; x < nodes.size(); x++) {
    ISerialized *deserializedDataNode = nodes[x]->Deserialize();
    if (deserializedDataNode)
      deserializedNodes.push_back(deserializedDataNode);

    // add instance
    ISerializedClass *serializedObject =
        dynamic_cast<ISerializedClass *>(deserializedDataNode);

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
      Statics::Destroy(dynamic_cast<ISerializedClass *>(deserializedNodes[x]));
    }

    // handle external node
    else if (deserializedNodes[x]->SerializedName() == "ExternalAsset") {
      LoadExternalAsset(deserializedNodes[x], fileName);
    }
  }
  
  IAssetManager *assetManager = Statics::Get<IAssetManager>();
  // helper types which are supposed to be deleted once the scene is loaded
  assetManager->RemoveAssetType("Entity");
  assetManager->RemoveAssetType("EntityIdCollection");
  assetManager->RemoveAssetType("ExternalAsset");

  // check if the ssd file contains entities
  if (entities.size() > 0)
    Statics::Get<ISceneManager>()->AddScene(fileName,
                                            SceneMaker::Create(nodes));
  else
    for (size_t x = 0; x < nodes.size(); x++)
      delete nodes[x];
  return true;
}
