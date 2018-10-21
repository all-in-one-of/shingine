#include <vector>
#include <map>

#include "Modules/Scene/CSceneMaker.h"
#include "Modules/Scene/CScene.h"

IScene* CSceneMaker::Create(const std::vector<IDataNode*> &nodes)
{
    CScene* scene = new CScene();
    scene->SetSerializedData(nodes);
    return scene;
}
