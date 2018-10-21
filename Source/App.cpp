#include <iostream>
#include "Core.h"
#include "Modules/Statics/CAssetManager.h"
#include "Modules/Statics/CComponentManager.h"
#include "Modules/Statics/CEntityManager.h"

int main()
{
    bool didLoad = CResourceLoader::Load("Assets/Scenes/TestScene.ssd");
    auto assets = CAssetManager::Get();
    auto components = CComponentManager::Get();
    auto entities = CEntityManager::Get();

    return 0;
}
