#include <vector>
#include <map>

#include "CSceneMaker.h"
#include "CSceneBuilder.h"
#include "CScene.h"
#include "../../IO/SceneDescription/SSD.h"

IScene* CSceneMaker::Create(SSD::SNode** nodes, const unsigned int &nodeCount)
{
    CScene* scene = new CScene();
    CSceneBuilder* sceneBuilder = new CSceneBuilder();
    std::map<unsigned int, unsigned int> descriptionIdToSceneId;
    // Create objects, attach components
    for (unsigned int x = 0; x < nodeCount; x++)
    {
        IObject* object = scene->MakeObject();
        descriptionIdToSceneId[nodes[x]->ID] = object->ID();
        sceneBuilder->SetupObjectFromDescriptionNode(object, nodes[x]);
    }

    // Setup parenting 
    for (unsigned int x = 0; x < nodeCount; x++)
    {
        scene->SetParent(
            descriptionIdToSceneId[nodes[x]->ID],
            descriptionIdToSceneId[nodes[x]->ParentID]
        );
    }
    
    delete sceneBuilder;
    return scene;
}