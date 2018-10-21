#include "Modules/Statics/CEntityManager.h"
#include "Modules/Statics/CStatics.h"
#include "Modules/Statics/CComponentManager.h"

CEntityManager* CEntityManager::Instance = NULL;

CEntityManager::CEntityManager()
{
}

unsigned int CEntityManager::CreateEntity()
{
    return CreateEntity(CStatics::Get()->GetUniqueId());
}

unsigned int CEntityManager::CreateEntity(unsigned int id)
{
    Ids.push_back(id);
    return id;
}

unsigned int CEntityManager::CreateEntity(std::vector<CString> componentsToAttach)
{
    unsigned int id = CreateEntity();
    for (size_t x = 0; x < componentsToAttach.size(); x++)
    {
        CComponentManager::Get()->AddComponent(componentsToAttach[x], id);
    }
    return id;
}

void CEntityManager::Destroy(unsigned int id)
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