#include "CScene.h"
#include "../Object/CSceneObject.h"

CScene::CScene()
{
}

void CScene::SetParent(const unsigned int &id, const unsigned int &parentId)
{
    CSceneObject* currentObject = (CSceneObject*) GetObject(id);
    CSceneObject* parentObject = (CSceneObject*) GetObject(parentId);
    if (currentObject)
        currentObject->SetParent(parentObject);
}

IObject* CScene::AddObject(const CString &objectName)
{
    CSceneObject* object = new CSceneObject(NextID);
    object->SetName(objectName);
    Objects[NextID] = object;
    NextID++;
    return object;
}

IObject* CScene::GetObject(const unsigned int &id) const
{
    auto it = Objects.find(id);
    if (it == Objects.end()) 
        return NULL;
    return it->second;
}

IObject* CScene::CloneObject(const IObject* object)
{
    return NULL;
}

unsigned int CScene::GetObjectIdByName(const CString &name, bool &didFind) const
{
    didFind = false;
    for (auto it = Objects.begin(); it != Objects.end(); it++)
    {
        if ( ((CSceneObject*)it->second)->Name() == name )
        {
            didFind = true;
            return it->second->ID();
        }
    }
    return 0;
}

void CScene::DeleteObject(const unsigned int &id)
{
}

void CScene::FindObjectsByType(const CString type, std::vector<IObject*> &objects) const
{
    for (auto it = Objects.begin(); it != Objects.end(); it++)
    {
        if (type == "_")
            objects.push_back(it->second);

    }

    return;
}

CScene::~CScene() 
{
}
