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

void CComponentManager::AddComponent(CString type, unsigned int entityId)
{
    ISerialized* serializedObject = CSerializedFactory::CreateInstance(type.GetStdString());
    if (!serializedObject) return;
    IComponent* component = dynamic_cast<IComponent*>(serializedObject);

    if (entityId != 0) 
        component->SetEntityId(entityId);
    AddComponent(component);
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
    auto it = Components.find(typeName.GetStdString());
    if (it == Components.end())
        return NULL;
    if (it->second.begin() == it->second.end())
        return NULL;
    if (componentId == 0)
        return it->second.begin()->second;

    auto it2 = it->second.find(componentId);
    if (it2 == it->second.end()) 
        return NULL;
    return it2->second;
}

void CComponentManager::GetAllComponents(std::vector<IComponent*> &components)
{
    auto it = Components.begin();
    for (it; it != Components.end(); it++)
    {
        auto it2 = it->second.begin();
        for (it2; it2 != it->second.end(); it2++)
        {
            components.push_back(it2->second);
        }
    }
}
void CComponentManager::GetComponentIteratorOfType(CString typeName, StringMap::iterator &iterator)
{
    auto it = Components.find(typeName.GetStdString());
    if (it == Components.end()) 
        return;
    iterator = it;
}
