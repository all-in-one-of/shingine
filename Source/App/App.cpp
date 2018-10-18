#include "../Core.h"
#include <iostream>

int main()
{
    IResourceLoader* ResourceLoader = CResourceLoader::Load("Assets/Scenes/TestScene.ssd");
    IScene* Scene = ResourceLoader->GenerateScene();

    delete ResourceLoader;
    return 0;
}
