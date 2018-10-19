#pragma once
#include "CSceneManager.h"
#include "CAssetManager.h"

class CStatics
{
public:
    static CSceneManager* SceneManager() { return CSceneManager::Get(); }
    static CAssetManager* AssetManager() { return CAssetManager::Get(); }
    static unsigned int GetUniqueId() 
    { 
        if (!Instance)
            Instance = new CStatics();
        return Instance->NextId++;
    }
    unsigned int NextId = 1000;
private:
    CStatics() {};
    static CStatics* Instance;
};
