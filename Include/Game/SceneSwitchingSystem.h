#pragma once
#include "Systems/System.h"
#include "Utility/Data/Serialization.h"
#include "Utility/Typedefs.h"

class SceneSwitchingSystem : public System, public IObject {
public:
  SERIALIZE_CLASS(SceneSwitchingSystem);
  ~SceneSwitchingSystem(){};
  virtual bool Initialize();
  virtual bool Update();

private:
  const unsigned char SceneCount = 3;
  unsigned char CurrentLoadedScene = 2;
  String ScenePaths[3] = {"Assets/Scenes/test0.ssd", "Assets/Scenes/test1.ssd",
                          "Assets/Scenes/test2.ssd"};
};
