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
        IObject* object = scene->MakeObject(CString(nodes[x]->Name()));
        descriptionIdToSceneId[nodes[x]->ID()] = object->ID();
        objectBuilder->SetupObjectFromDescriptionNode(object, nodes[x]);
    }

    // Setup parenting 
    for (unsigned int x = 0; x < nodes.size(); x++)
    {
        scene->SetParent(
            descriptionIdToSceneId[nodes[x]->ID()],
            descriptionIdToSceneId[nodes[x]->ParentID()]
        );
    }
    
    delete objectBuilder;
    return scene;
}