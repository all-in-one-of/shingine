#include <vector>
#include <map>

#include "Modules/Scene/CSceneMaker.h"
#include "Modules/Scene/CScene.h"

IScene* CSceneMaker::Create(const std::vector<IObject*> &objects)
{
    CScene* scene = new CScene();
    // Create objects, attach components
    for (unsigned int x = 0; x < objects.size(); x++)
        scene->AddCreatedObject(objects[x]);
    return scene;
}
