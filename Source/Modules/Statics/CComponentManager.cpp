#include "Modules/Statics/CComponentManager.h"
#include "Utility/Data/ISerialized.h"
#include "Engine/IComponent.h"
#include "Utility/Data/CSerializedFactory.h"
CComponentManager* CComponentManager::Instance = NULL;

CComponentManager::CComponentManager()
{
}

void CComponentManager::AddComponent(IComponent* component)
{
    unsigned int entityId = component->EntityId();
    if (entityId == 0) 
        entityId = component->Id();
    
    Components[dynamic_cast<ISerializedClass*>(component)->
        SerializedName().GetStdString()][entityId] = component;
}

IComponent* CComponentManager::AddComponent(CString type, unsigned int entityId)
{
    ISerialized* serializedObject = CSerializedFactory::CreateInstance(type.GetStdString());
    if (!serializedObject) 
        return NULL;
    IComponent* component = dynamic_cast<IComponent*>(serializedObject);

    if (entityId != 0) 
        component->SetEntityId(entityId);
    AddComponent(component);
    return component;
}

void CComponentManager::UpdateComponentEntityId(IComponent* component)
{
    // get component name
    ISerializedClass* serializedComponent = dynamic_cast<ISerializedClass*>(component);
    unsigned int uniqueId = serializedComponent->UniqueID();
    Components[serializedComponent->SerializedName().GetStdString()].erase(uniqueId);
    Components[serializedComponent->SerializedName().GetStdString()][component->EntityId()] = component;
}

IComponent* CComponentManager::GetComponentOfType(CString typeName, unsigned int componentId)
{
    StringMap::iterator it = Components.find(typeName.GetStdString());
    if (it == Components.end())
        return NULL;

    IdMap &idMap = it->second;

    if (idMap.begin() == idMap.end())
        return NULL;
    if (componentId == 0)
        return idMap.begin()->second;

    IdMap::iterator idMapIterator = idMap.find(componentId);
    if (idMapIterator == idMap.end()) 
        return NULL;

    return idMapIterator->second;
}

void CComponentManager::GetAllComponents(std::vector<IComponent*> &components)
{
    for (StringMap::iterator it = Components.begin(); it != Components.end(); it++)
    {
        IdMap &idMap = it->second;
        for (IdMap::iterator idMapIt = idMap.begin(); idMapIt != idMap.end(); idMapIt++)
            components.push_back(idMapIt->second);
    }
}
bool CComponentManager::GetComponentIteratorOfType(CString typeName, StringMap::iterator &iterator)
{
    StringMap::iterator it = Components.find(typeName.GetStdString());
    if (it == Components.end()) 
        return false;
    iterator = it;
    return true;
}
