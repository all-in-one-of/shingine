#pragma once
#include "Statics.h"
#include <map>
class IScene;

class ISceneManager {
public:
  virtual ~ISceneManager() {}
  virtual bool LoadScene(const String &fileName) = 0;
  virtual void SetCurrentScene(const String &fileName) = 0;
  virtual String GetCurrentSceneFileName() = 0;
  virtual String
  GetExternalAssetPathRelativeToTheSceneFile(const String &assetFileName) = 0;
};
