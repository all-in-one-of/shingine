#pragma once
#include <vector>
#include "Core.h"

class EntityManager
{
public:
    static EntityManager* Get() 
    {
        if (!Instance) Instance = new EntityManager();
        return Instance;
    }
    unsigned int CreateEntity();
    unsigned int CreateEntity(unsigned int id);
    unsigned int CreateEntity(std::vector<String> componentsToAttach);
    void Destroy(unsigned int id);
private:
    EntityManager();
    static EntityManager* Instance;
    std::vector<unsigned int> Ids;
};
