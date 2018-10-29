#include <vector>
#include <map>

#include "Modules/Scene/SceneMaker.h"
#include "Modules/Scene/Scene.h"

IScene* SceneMaker::Create(const std::vector<IDataNode*> &nodes)
{
    Scene* scene = new Scene();
    scene->SetSerializedData(nodes);
    return scene;
}
