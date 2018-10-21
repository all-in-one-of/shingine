#include "Modules/Statics/CInstanceManager.h"
#include "Utility/Data/ISerialized.h"
#include "Engine/IComponent.h"

CInstanceManager* CInstanceManager::Instance = NULL;

CInstanceManager::CInstanceManager()
{
}

void CInstanceManager::AddInstance(ISerializedClass* newAsset)
{
    // check if a component or an asset
    IComponent* component = dynamic_cast<IComponent*>(newAsset);
    if (component)
        Components[newAsset->SerializedName().GetStdString()][newAsset->UniqueID()] = component;
    else
        Assets[newAsset->SerializedName().GetStdString()][newAsset->UniqueID()] = newAsset;
}

#define __GET_SOME_THING_OF_TYPE(THING) \
    auto it = THING.find(typeName.GetStdString()); \
    if (it == THING.end()) \
        return NULL; \
    if (it->second.begin() == it->second.end()) \
        return NULL; \
    return it->second.begin()->second;

ISerializedClass* CInstanceManager::GetAssetOfType(CString typeName)
{
    __GET_SOME_THING_OF_TYPE(Assets)
}

IComponent* CInstanceManager::GetComponentOfType(CString typeName)
{
    __GET_SOME_THING_OF_TYPE(Components)
}


void CInstanceManager::AddEntityId(unsigned int entityId)
{
    Ids.push_back(entityId);
}

void CInstanceManager::GetAllComponents(std::vector<IComponent*> &components)
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

void CInstanceManager::Destroy(ISerializedClass* obj)
{
    if (obj)
        delete obj;
}

void CInstanceManager::Destroy(unsigned int entityId)
{
    int deleteIndex = -1;
    for (int x = 0; x < Ids.size(); x++)
    {
        if (Ids[x] == entityId) 
        {
            deleteIndex = x;
            break;
        }
    }

    if (deleteIndex != -1)
        Ids.erase(Ids.begin() + deleteIndex);

    // TODO remove components which have this entity id
}

unsigned int CInstanceManager::GetUniqueId() 
{ 
    if (!Instance)
        Instance = new CInstanceManager();
    return Instance->NextId++;
}
