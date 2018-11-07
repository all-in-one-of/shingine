#include <fstream>
#include <iostream>
#include <stdio.h>

#include "Modules/ResourceLoader/CResourceReaderFactory.h"
#include "Modules/ResourceLoader/ResourceLoader.h"

#include "Modules/Scene/SceneMaker.h"
#include "Utility/Data/ISerialized.h"
#include "Utility/Data/UniqueIdSetter.h"

#include "Modules/Statics/IAssetManager.h"

#include "Engine/AssetTypes/Asset.h"
#include "Modules/ResourceLoader/BitmapReader/BitmapReader.h"

String ResourceLoader::LastError = "";
String ResourceLoader::GetLastError() { return LastError; }

void ResourceLoader::SetupPath(const String &localPath, String &outPath) {
  const String basePath = "";
  outPath = basePath + localPath;
}

bool ResourceLoader::LoadBitmap(const String &fileName,
                                IObject *&textureAsset) {
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

bool ResourceLoader::LoadAsset(const String &fileName, Asset *&loadedAsset,
                               unsigned int uniqueId) {
  std::vector<IDataNode *> nodes;
  if (!LoadSsd(fileName, nodes))
    return false;
  assert(nodes.size() == 1); // loading a single asset file

  if (uniqueId == 0)
    UniqueIdSetter::SetIds(nodes);

  ISerialized *deserializedObj = nodes[0]->Deserialize();
  IObject *serializedClass = dynamic_cast<IObject *>(deserializedObj);
  // TODO fix
  // if it's a complex asset which contains serialized objects inside,
  // those won't get a global unique id.
  if (uniqueId != 0)
    serializedClass->SetUniqueID(uniqueId);

  Statics::AddSerializedObject(serializedClass);
  loadedAsset = dynamic_cast<Asset *>(serializedClass);
  assert(loadedAsset != NULL);
  // set run time parameters
  loadedAsset->SetOrigin(Asset::OriginType::External);
  loadedAsset->SetFileName(fileName);
  Statics::Get<IAssetManager>()->SaveExternalAssetPath(
      fileName, serializedClass->UniqueID());
  // clean up
  delete nodes[0];
  return true;
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
  if (reader == nullptr)
    return false;
  
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
