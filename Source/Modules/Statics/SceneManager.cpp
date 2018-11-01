#include "Modules/Statics/SceneManager.h"

REGISTER_SERIALIZED_CLASS(SceneManager)
SceneManager::SceneManager(){};

void SceneManager::SetCurrentScene(const String &fileName) {
  CurrentSceneFileName = fileName;
}
String SceneManager::GetCurrentSceneFileName() { return CurrentSceneFileName; }