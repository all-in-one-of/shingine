#include "../Core.h"
#include <iostream>

int main()
{
    bool sceneDescriptionLoaded = false;
    ISceneDescription* SceneDescription = CSceneDescription::Load("Assets/Scenes/test.ssd", sceneDescriptionLoaded);
    IScene* Scene = SceneDescription->GenerateScene();
    
    return 0;
}
