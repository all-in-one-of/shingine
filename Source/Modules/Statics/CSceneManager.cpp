#include "Modules/Statics/CSceneManager.h"

CSceneManager* CSceneManager::Instance = NULL;

void CSceneManager::AddScene(const CString &fileName, IScene* scene)
{
    Scenes[fileName] = scene;
    CurrentSceneInstance = scene;
}

IScene* CSceneManager::CurrentScene() { return CurrentSceneInstance; }

CSceneManager::CSceneManager() 
{
};
