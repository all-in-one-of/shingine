#include "Modules/Statics/SceneManager.h"

REGISTER_SERIALIZED_CLASS(SceneManager)

void SceneManager::AddScene(const String &fileName, IScene *scene) {
  Scenes[fileName] = scene;
  CurrentSceneInstance = scene;
}

IScene *SceneManager::CurrentScene() { return CurrentSceneInstance; }

SceneManager::SceneManager(){};
