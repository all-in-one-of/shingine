#pragma once
#include "ISceneManager.h"
#include "Utility/Data/Serialization.h"
#include <map>

class SceneManager : public ISceneManager, public ISerializedClass {
public:
  SERIALIZE_CLASS(SceneManager);
  SceneManager();
  virtual ~SceneManager() {}
  virtual void SetCurrentScene(const String &fileName);
  virtual String GetCurrentSceneFileName();

private:
  String CurrentSceneFileName = "";
};
