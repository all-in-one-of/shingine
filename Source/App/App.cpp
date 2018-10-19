#include "Core.h"
#include "Modules/Statics/CStatics.h"
#include <iostream>

int main()
{
    bool didLoad = CResourceLoader::Load("Assets/Scenes/TestScene.ssd");
    //IScene* Scene = ResourceLoader->GenerateScene();
    CAssetManager* assetManager = CStatics::AssetManager();
    CSceneManager* sceneManager = CStatics::SceneManager();
    return 0;
}
