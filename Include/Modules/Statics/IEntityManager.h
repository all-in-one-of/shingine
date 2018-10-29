#pragma once
#include <vector>
#include "Statics.h"

class String;
class IEntityManager
{
public:
    virtual ~IEntityManager() {}
    virtual unsigned int CreateEntity() = 0;
    virtual unsigned int CreateEntity(unsigned int id) = 0;
    virtual unsigned int CreateEntity(std::vector<String> componentsToAttach) = 0;
    virtual void Destroy(unsigned int id) = 0;
};
