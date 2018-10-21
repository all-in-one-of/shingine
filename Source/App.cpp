#include <iostream>
#include "Core.h"
#include "Modules/Statics/CStatics.h"

int main()
{
    bool didLoad = CResourceLoader::Load("Assets/Scenes/TestScene.ssd");
    CInstanceManager* assetManager = CStatics::InstanceManager();
    CSceneManager* sceneManager = CStatics::SceneManager();
    return 0;
}
