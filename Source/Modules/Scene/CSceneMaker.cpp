#include <vector>
#include <map>

#include "CSceneMaker.h"
#include "CScene.h"
#include "../Object/CObjectBuilder.h"
#include "../../Utility/Data/IDataNode.h"

IScene* CSceneMaker::Create(const std::vector<IDataNode*> &nodes)
{
    CScene* scene = new CScene();
    CObjectBuilder* objectBuilder = new CObjectBuilder();
    std::map<unsigned int, unsigned int> descriptionIdToSceneId;
    // Create objects, attach components
    for (unsigned int x = 0; x < nodes.size(); x++)
    {
        IObject* object = scene->AddObject(CString(nodes[x]->Name()));
        descriptionIdToSceneId[nodes[x]->ID()] = object->ID();
        objectBuilder->SetupFromDataNode(object, nodes[x]);
    }
    
    delete objectBuilder;
    return scene;
}