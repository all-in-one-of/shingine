#pragma once
#include <vector>
class String;
class ISerialized;
class ISerializedClass;

class ResourceLoader {
public:
  static bool Load(const String &fileName);
  static bool LoadText(const String &fileName, String &data);
  static bool LoadBitmap(const String &fileName,
                         ISerializedClass *&textureAsset);
  static String GetLastError();

private:
  ResourceLoader(){};
  static void SetupPath(const String &localPath, String &outPath);
  static String LastError;
};
