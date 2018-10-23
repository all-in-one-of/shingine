#pragma once
#include <unordered_map>
#include "Core.h"

class IComponent;

class CComponentManager
{
public:
    typedef std::unordered_map<unsigned int, IComponent*> IdMap;
    typedef std::unordered_map<std::string, IdMap> StringMap;

    static CComponentManager* Get() 
    {
        if (!Instance) Instance = new CComponentManager();
        return Instance;
    }

    void AddComponent(IComponent* component);
    IComponent* AddComponent(CString type, unsigned int entityId = 0);
    void UpdateComponentEntityId(IComponent* component);
    IComponent* GetComponentOfType(CString typeName, unsigned int componentId = 0);
    
    void GetAllComponents(std::vector<IComponent*> &components);
    bool GetComponentIteratorOfType(CString typeName, StringMap::iterator &iterator);

    template<class T>
    T* GetComponentOfType(CString typeName, unsigned int componentId = 0)
    {
        IComponent* component = GetComponentOfType(typeName, componentId);
        return dynamic_cast<T*>(component);
    }

    template<class T>
    T* AddComponent(CString typeName)
    {
        IComponent* component = AddComponent(typeName);
        return dynamic_cast<T*>(component);
    }

private:
    CComponentManager();
    static CComponentManager* Instance;
    StringMap Components;

    
};
