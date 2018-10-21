#pragma once
#include <unordered_map>
#include <string>
#include "Core.h"

class IComponent;
class ISerializedClass;

class CInstanceManager
{
public:
    static CInstanceManager* Get()
    {
        if (!CInstanceManager::Instance)
            CInstanceManager::Instance = new CInstanceManager();
        return CInstanceManager::Instance;
    }
    void AddEntityId(unsigned int entityId);
    void AddInstance(ISerializedClass* newAsset);

    static unsigned int GetUniqueId();
    ISerializedClass* GetAssetOfType(CString typeName);
    IComponent* GetComponentOfType(CString typeName);
    void GetAllComponents(std::vector<IComponent*> &components);

    void Destroy(ISerializedClass* obj);
    void Destroy(unsigned int entityId);
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
