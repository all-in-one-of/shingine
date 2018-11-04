#pragma once
#include <vector>
class String;
class ISerialized;
class IObject;
class Asset;

class ResourceLoader {
public:
  static bool LoadSsd(const String &fileName,
                      std::vector<class IDataNode *> &nodes);
  // static bool LoadScene(const String &fileName);
  static bool LoadAsset(const String &fileName, Asset *&loadedAsset,
                        unsigned int uniqueId = 0);
  static bool LoadText(const String &fileName, String &data);
  static bool LoadBitmap(const String &fileName,
                         IObject *&textureAsset);
  static String GetLastError();

private:
  ResourceLoader(){};
  static void LoadExternalAssetFromScene(ISerialized *obj,
                                         const String &sceneFileName);
  static void SetupPath(const String &localPath, String &outPath);
  static String LastError;
};
