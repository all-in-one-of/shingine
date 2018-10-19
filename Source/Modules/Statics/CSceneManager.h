#pragma once
#include <map>
#include "../Scene/IScene.h"
class CSceneManager
{
public:
    static CSceneManager* Get()
    {
        if (!Instance)
            Instance = new CSceneManager();
        return Instance;
    }

    void AddScene(const CString &fileName, IScene* scene)
    {
        Scenes[fileName] = scene;
        CurrentSceneInstance = scene;
    }

    IScene* CurrentScene() { return CurrentSceneInstance; }
private:
    CSceneManager() {};
    static CSceneManager* Instance;
    std::map<CString, IScene*> Scenes;
    IScene* CurrentSceneInstance;
};
