#include "Game/SceneSwitchingSystem.h"
#include "Core.h"
#include "Modules/Statics/IInput.h"
#include "Modules/Statics/ISceneManager.h"

#include <iostream>

REGISTER_SERIALIZED_CLASS(SceneSwitchingSystem)

ISceneManager *SceneManager = nullptr;
bool SceneSwitchingSystem::Initialize() {
  // Array of scenes
  // load scene 0
  SceneManager = Statics::Get<ISceneManager>();
  std::cout << "Loading scene " << ScenePaths[CurrentLoadedScene] << std::endl;
  SceneManager->LoadScene(ScenePaths[CurrentLoadedScene]);
  
  return true;
}
bool SceneSwitchingSystem::Update() {
  IInput *input = Statics::Get<IInput>();
  if (input->GetKeyUp(S_INPUT_KEY_EQUAL)) {
    CurrentLoadedScene = (CurrentLoadedScene + 1) % SceneCount;
    std::cout << "Triggering the scene switch" << std::endl;
    SceneManager->LoadScene(ScenePaths[CurrentLoadedScene]);
  }
  return true;
}
