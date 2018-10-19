#include "Modules/Scene/CScene.h"

CScene::CScene()
{
}

void CScene::SetParent(const unsigned int &id, const unsigned int &parentId)
{
    // CSceneObject* currentObject = (CSceneObject*) GetObject(id);
    // CSceneObject* parentObject = (CSceneObject*) GetObject(parentId);
    // if (currentObject)
    //     currentObject->SetParent(parentObject);
}

void CScene::GetComponents(const CString &type, std::map<unsigned int, IComponent*> &componentMap) const
{
    std::map<unsigned int, IObject*>::const_iterator it;
    for (it = Objects.begin(); it != Objects.end(); it++)
    {
        IComponent* component = it->second->GetComponent(type);
        if (component)
            componentMap[it->first] = component;
    }
}

void CScene::GetComponents(const CString &type, std::vector<IComponent*> &components) const
{
    std::map<unsigned int, IObject*>::const_iterator it;
    for (it = Objects.begin(); it != Objects.end(); it++)
    {
        IComponent* component = it->second->GetComponent(type);
        if (component)
            components.push_back(component);
    }
}

void CScene::AddCreatedObject(IObject* object)
{
    std::map<unsigned int, IObject*>::iterator it;
    for (it = Objects.begin(); it != Objects.end(); it++)
    {
        if (it->second == object) return;
    }
    NextID = object->ID() > NextID ? object->ID() + 1 : NextID + 1;
    Objects[object->ID()] = object;
}

IObject* CScene::AddObject(const CString &objectName)
{
    //CObject* object = new CObject();
    //object->SetName(objectName);
    //Objects[NextID] = object;
    //NextID++;
    //return object;
    return NULL;
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
    std::map<unsigned int, IObject*>::const_iterator it;
    for (it = Objects.begin(); it != Objects.end(); it++)
    {
        // if ( ((CSceneObject*)it->second)->Name() == name )
        // {
        //     didFind = true;
        //     return it->second->ID();
        // }
    }
    return 0;
}

void CScene::DeleteObject(const unsigned int &id)
{
}

void CScene::FindObjectsByType(const CString type, std::vector<IObject*> &objects) const
{
    std::map<unsigned int, IObject*>::const_iterator it;
    for (it = Objects.begin(); it != Objects.end(); it++)
    {
        if (type == "_")
            objects.push_back(it->second);

    }

    return;
}

CScene::~CScene() 
{
}
