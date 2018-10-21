#pragma once
#include <unordered_map>
#include <string>
#include "Core.h"

class IComponent;
class ISerializedClass;

typedef std::unordered_map<std::string, std::unordered_map<unsigned int, IComponent*>>::iterator IComponentIterator;

class CInstanceManager
{
public:
    // static CInstanceManager* Get()
    // {
    //     if (!CInstanceManager::Instance)
    //         CInstanceManager::Instance = new CInstanceManager();
    //     return CInstanceManager::Instance;
    // }

    // unsigned int AddEntity();

    // void AddEntityId(unsigned int entityId);
    // void AddInstance(ISerializedClass* newAsset);

    // static unsigned int GetUniqueId();
    // void UpdateComponentEntityId(IComponent* component);

    // ISerializedClass* GetAssetOfType(CString typeName, unsigned int assetId = 0);
    // IComponent* GetComponentOfType(CString typeName, unsigned int componentId = 0);

    // void GetAllComponents(std::vector<IComponent*> &components);

    // void GetComponentIteratorOfType(CString typeName, IComponentIterator &iterator);

    // void Destroy(ISerializedClass* obj);
    // void Destroy(unsigned int entityId);
private:
    CInstanceManager();
    // ids
    std::vector<unsigned int> Ids;
    // components
    std::unordered_map<std::string, std::unordered_map<unsigned int, IComponent*>> Components;
    // assets
    std::unordered_map<std::string, std::unordered_map<unsigned int, ISerializedClass*>> Assets;
    
    static CInstanceManager* Instance;
    unsigned int NextId = 1000;
};
