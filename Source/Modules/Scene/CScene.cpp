#include "CSceneBuilder.h"
#include "CScene.h"
#include "../Object/CObject.h"

IScene* CScene::Create(SSD::SNode** nodes, const unsigned int &nodeCount)
{
    CScene* scene = new CScene();
    CSceneBuilder* sceneBuilder = new CSceneBuilder();

    for (unsigned int x = 0; x < nodeCount; x++)
    {

    }
    
    delete sceneBuilder;
    return scene;
}

CScene::CScene()
{
}

void CScene::SetParent(const unsigned int &id, const unsigned int &parentId)
{
    
}

IObject* CScene::MakeObject()
{
    return NULL;
}

IObject* CScene::GetObject(const unsigned int &id) const
{
    return NULL;
}

IObject* CScene::CloneObject(const IObject* object)
{
    return NULL;
}

void CScene::GetObjectIdByName(unsigned int &id, bool &didFind) const
{
}

void CScene::DeleteObject(const unsigned int &id)
{
}

std::vector<IObject*> CScene::FindObjectsByType(const CString type) const
{
    return std::vector<IObject*>();
}

CScene::~CScene() 
{
}
