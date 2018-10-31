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

bool ResourceLoader::Load(const String &fileName) {
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

  std::vector<ISerialized *> deserializedNodes;
  std::vector<IDataNode *> nodes;
  reader->ReadNodes(nodes);

  // traverse through each node, create unique ids
  UniqueIdSetter::SetIds(nodes);

  for (size_t x = 0; x < nodes.size(); x++) {
    ISerialized *deserializedDataNode = nodes[x]->Deserialize();
    if (deserializedDataNode)
      deserializedNodes.push_back(deserializedDataNode);

    // add instance
    ISerializedClass *serializedObject =
        dynamic_cast<ISerializedClass *>(deserializedDataNode);
    Statics::AddSerializedObject(serializedObject);
  }

  reader->Close();
  delete reader;

  std::vector<ISerialized *> entities;

  for (unsigned int x = 0; x < deserializedNodes.size(); x++) {
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
  }

  Statics::Get<IAssetManager>()->RemoveAssetType("Entity");
  Statics::Get<IAssetManager>()->RemoveAssetType("EntityIdCollection");

  // check if the ssd file contains entities
  if (entities.size() > 0)
    Statics::Get<ISceneManager>()->AddScene(fileName,
                                            SceneMaker::Create(nodes));
  else
    for (size_t x = 0; x < nodes.size(); x++)
      delete nodes[x];
  return true;
}
