#include "../Core.h"
#include <iostream>

int main()
{
    ISceneDescription* SceneDescription = CSceneDescription::Load("Assets/Scenes/test.ssd");
    std::cout << "Hello" << std::endl;
    return 0;
}
