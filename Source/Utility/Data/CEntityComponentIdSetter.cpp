#include <unordered_map>
#include "Utility/Data/CEntityComponentIdSetter.h"
#include "Utility/Data/ISerialized.h"
#include "Engine/CEntity.h"
#include "Engine/IComponent.h"

#include "Modules/Statics/CInstanceManager.h"

void CEntityComponentIdSetter::UpdateIds(ISerialized* idCollections)
{
    CEntityIdCollection* collection = dynamic_cast<CEntityIdCollection*>(idCollections);
    if (!collection) 
        return;
    CEntityComponentIdSetter s = CEntityComponentIdSetter(collection);

}

CEntityComponentIdSetter::CEntityComponentIdSetter(CEntityIdCollection* collection)
{
    std::unordered_map<unsigned int, unsigned int> componentIdToEntityId;
    CInstanceManager* instanceManager = CInstanceManager::Get();
    for (size_t x = 0; x < collection->Ids.size(); x++)
    {
        unsigned int entityId = collection->Ids[x];
        instanceManager->AddEntityId(entityId);
        for (size_t y = 0; y < collection->Components[x]->Ids.size(); y++)
            componentIdToEntityId[collection->Components[x]->Ids[y]] = entityId;
    }
    std::vector<IComponent*> components;
    instanceManager->GetAllComponents(components);
    for (size_t x = 0; x < components.size(); x++)
    {
        unsigned int componentId = components[x]->Id();
        auto it = componentIdToEntityId.find(componentId);
        if (it == componentIdToEntityId.end())
            continue;
        components[x]->SetEntityId(it->second);
    }
}
