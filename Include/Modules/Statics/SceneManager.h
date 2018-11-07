#pragma once
#include "ISceneManager.h"
#include "Utility/Data/Serialization.h"
#include <map>

class SceneManager : public ISceneManager, public IObject {
public:
  SERIALIZE_CLASS(SceneManager);
  SceneManager();
  virtual ~SceneManager() {}
  virtual bool LoadScene(const String &fileName);
  virtual void SetCurrentScene(const String &fileName);
  virtual String GetCurrentSceneFileName();
  String
  GetExternalAssetPathRelativeToTheSceneFile(const String &assetFileName);

private:
  bool UnloadCurrentScene();
  void LoadExternalAssetFromScene(ISerialized *obj, const String &sceneFileName,
                                  class UniqueIdSetter *uidSetter);
  String
  GetExternalAssetPathRelativeToTheSceneFile(const String &assetFileName,
                                             const String &sceneFileName);
  String CurrentSceneFileName = "";
};
