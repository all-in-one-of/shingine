#include <vector>
#include <map>

#include "CSceneMaker.h"
#include "CScene.h"
#include "../Object/CObjectBuilder.h"
#include "../../Utility/SSD.h"

IScene* CSceneMaker::Create(SSD::SNode** nodes, const unsigned int &nodeCount)
{
    CScene* scene = new CScene();
    CObjectBuilder* objectBuilder = new CObjectBuilder();
    std::map<unsigned int, unsigned int> descriptionIdToSceneId;
    // Create objects, attach components
    for (unsigned int x = 0; x < nodeCount; x++)
    {
        IObject* object = scene->MakeObject();
        descriptionIdToSceneId[nodes[x]->ID] = object->ID();
        objectBuilder->SetupObjectFromDescriptionNode(object, nodes[x]);
    }

    // Setup parenting 
    for (unsigned int x = 0; x < nodeCount; x++)
    {
        scene->SetParent(
            descriptionIdToSceneId[nodes[x]->ID],
            descriptionIdToSceneId[nodes[x]->ParentID]
        );
    }
    
    delete objectBuilder;
    return scene;
}