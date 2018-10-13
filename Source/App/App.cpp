#include "../Core.h"
#include <iostream>

int main()
{
    bool sceneDescriptionLoaded = false;
    ISceneDescription* sceneDescription = CSceneDescription::Load("Assets/Scenes/test.ssd", sceneDescriptionLoaded);
    IScene* Scene = sceneDescription->GenerateScene();
    
    delete sceneDescription;
    return 0;
}
