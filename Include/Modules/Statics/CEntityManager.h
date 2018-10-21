#pragma once
#include <vector>
#include "Core.h"

class CEntityManager
{
public:
    static CEntityManager* Get() 
    {
        if (!Instance) Instance = new CEntityManager();
        return Instance;
    }
    unsigned int CreateEntity();
    unsigned int CreateEntity(unsigned int id);
    unsigned int CreateEntity(std::vector<CString> componentsToAttach);
    void Destroy(unsigned int id);
private:
    CEntityManager();
    static CEntityManager* Instance;
    std::vector<unsigned int> Ids;
};
