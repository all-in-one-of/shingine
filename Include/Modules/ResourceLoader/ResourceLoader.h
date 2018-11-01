#pragma once
#include <vector>
class String;
class ISerialized;
class ISerializedClass;
class Asset;

class ResourceLoader {
public:
  static bool LoadScene(const String &fileName);
  static bool LoadAsset(const String &fileName, Asset *&loadedAsset,
                        unsigned int uniqueId = 0);
  static bool LoadText(const String &fileName, String &data);
  static bool LoadBitmap(const String &fileName,
                         ISerializedClass *&textureAsset);
  static String GetLastError();

private:
  ResourceLoader(){};
  static bool LoadSsd(const String &fileName,
                      std::vector<class IDataNode *> &nodes);
  static void LoadExternalAssetFromScene(ISerialized *obj,
                                         const String &sceneFileName);
  static void SetupPath(const String &localPath, String &outPath);
  static String LastError;
};
