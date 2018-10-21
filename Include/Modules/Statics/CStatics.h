#pragma once
#include "CSceneManager.h"
#include "CInstanceManager.h"

class CStatics
{
public:
    static CSceneManager* SceneManager();
    static CInstanceManager* InstanceManager() { return CInstanceManager::Get(); }
private:
    CStatics() {};
    static CStatics* Instance;
};
