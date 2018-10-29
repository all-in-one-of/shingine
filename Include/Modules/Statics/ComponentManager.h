#pragma once
#include <unordered_map>
#include <typeinfo>
#include "Utility/Data/SerializedFactory.h"
#include "Core.h"

class IComponent;

class ComponentManager
{
public:
    typedef std::unordered_map<unsigned int, IComponent*> IdMap;
    typedef std::unordered_map<std::string, IdMap> StringMap;

    static ComponentManager* Get() 
    {
        if (!Instance) Instance = new ComponentManager();
        return Instance;
    }

    void AddComponent(IComponent* component);
    IComponent* AddComponent(String type, unsigned int entityId = 0);
    void UpdateComponentEntityId(IComponent* component);
    IComponent* GetComponentOfType(String typeName, unsigned int componentId = 0);
    
    void GetAllComponents(std::vector<IComponent*> &components);
    bool GetComponentIteratorOfType(String typeName, StringMap::iterator &iterator);

    template<class T>
    T* GetComponentOfType(unsigned int componentId = 0)
    {
        String typeName = typeid(T).name();
        SerializedFactory::GetDemangledName(typeName);
        IComponent* component = GetComponentOfType(typeName, componentId);
        return dynamic_cast<T*>(component);
    }

    template<class T>
    T* AddComponent(unsigned int entityId = 0)
    {
        String typeName = typeid(T).name();
        SerializedFactory::GetDemangledName(typeName);
        IComponent* component = AddComponent(typeName, entityId);
        return dynamic_cast<T*>(component);
    }

private:
    ComponentManager();
    static ComponentManager* Instance;
    StringMap Components;

    
};
