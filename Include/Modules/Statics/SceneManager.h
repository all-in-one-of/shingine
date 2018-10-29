#pragma once
#include <map>
#include "../Scene/IScene.h"

class SceneManager
{
public:
    static SceneManager* Get()
    {
        if (!SceneManager::Instance)
            SceneManager::Instance = new SceneManager();
        return SceneManager::Instance;
    }
    void AddScene(const String &fileName, IScene* scene);
    IScene* CurrentScene();
private:
    SceneManager();
    static SceneManager* Instance;
    std::map<String, IScene*> Scenes;
    IScene* CurrentSceneInstance = NULL;
};
