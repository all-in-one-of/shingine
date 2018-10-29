#include "Modules/Statics/SceneManager.h"

SceneManager* SceneManager::Instance = NULL;

void SceneManager::AddScene(const String &fileName, IScene* scene)
{
    Scenes[fileName] = scene;
    CurrentSceneInstance = scene;
}

IScene* SceneManager::CurrentScene() { return CurrentSceneInstance; }

SceneManager::SceneManager() 
{
};
