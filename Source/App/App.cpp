#include "../Core.h"
#include <iostream>

int main()
{
    ISceneDescription* sceneDescription = CSceneDescription::Load("Assets/Scenes/SingleObject.ssd");
    IScene* Scene = sceneDescription->GenerateScene();
    
    delete sceneDescription;
    return 0;
}
