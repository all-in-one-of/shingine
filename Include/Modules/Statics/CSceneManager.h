#pragma once
#include <map>
#include "../Scene/IScene.h"

class CSceneManager
{
public:
    static CSceneManager* Get()
    {
        if (!CSceneManager::Instance)
            CSceneManager::Instance = new CSceneManager();
        return CSceneManager::Instance;
    }
    void AddScene(const CString &fileName, IScene* scene);
    IScene* CurrentScene();
private:
    CSceneManager();
    static CSceneManager* Instance;
    std::map<CString, IScene*> Scenes;
    IScene* CurrentSceneInstance;
};
