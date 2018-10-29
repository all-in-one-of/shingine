#include "Modules/Statics/EntityManager.h"
#include "Modules/Statics/Statics.h"
#include "Modules/Statics/ComponentManager.h"

EntityManager* EntityManager::Instance = NULL;

EntityManager::EntityManager()
{
}

unsigned int EntityManager::CreateEntity()
{
    return CreateEntity(Statics::Get()->GetUniqueId());
}

unsigned int EntityManager::CreateEntity(unsigned int id)
{
    Ids.push_back(id);
    return id;
}

unsigned int EntityManager::CreateEntity(std::vector<String> componentsToAttach)
{
    unsigned int id = CreateEntity();
    for (size_t x = 0; x < componentsToAttach.size(); x++)
    {
        ComponentManager::Get()->AddComponent(componentsToAttach[x], id);
    }
    return id;
}

void EntityManager::Destroy(unsigned int id)
{
    int deleteIndex = -1;
    for (int x = 0; x < Ids.size(); x++)
    {
        if (Ids[x] == id) 
        {
            deleteIndex = x;
            break;
        }
    }

    if (deleteIndex != -1)
        Ids.erase(Ids.begin() + deleteIndex);

}
