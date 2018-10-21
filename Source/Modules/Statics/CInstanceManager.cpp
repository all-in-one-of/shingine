// #include "Modules/Statics/CInstanceManager.h"
// #include "Utility/Data/ISerialized.h"
// #include "Engine/IComponent.h"

// CInstanceManager* CInstanceManager::Instance = NULL;

// CInstanceManager::CInstanceManager()
// {
// }

// // void CInstanceManager::AddInstance(ISerializedClass* newAsset)
// // {
// //     // check if a component or an asset
// //     IComponent* component = dynamic_cast<IComponent*>(newAsset);
// //     if (component)
// //         Components[newAsset->SerializedName().GetStdString()][newAsset->UniqueID()] = component;
// //     else
// //         Assets[newAsset->SerializedName().GetStdString()][newAsset->UniqueID()] = newAsset;
// // }

// // void CInstanceManager::UpdateComponentEntityId(IComponent* component)
// // {
// //     // get component name
// //     ISerializedClass* serializedComponent = dynamic_cast<ISerializedClass*>(component);
// //     unsigned int uniqueId = serializedComponent->UniqueID();
// //     Components[serializedComponent->SerializedName().GetStdString()].erase(uniqueId);
// //     Components[serializedComponent->SerializedName().GetStdString()][component->EntityId()] = component;
// // }

// // void CInstanceManager::GetComponentIteratorOfType(CString typeName, std::unordered_map<std::string, std::unordered_map<unsigned int, IComponent*>>::iterator &iterator)
// // {
// //     auto it = Components.find(typeName.GetStdString());
// //     if (it == Components.end()) 
// //         return;
// //     iterator = it;
// // }

// #define __GET_SOME_THING_OF_TYPE(THING) \

// ISerializedClass* CInstanceManager::GetAssetOfType(CString typeName, unsigned int assetId)
// {
//     auto it = Assets.find(typeName.GetStdString());
//     if (it == Assets.end())
//         return NULL;
//     if (it->second.begin() == it->second.end())
//         return NULL;
//     if (assetId == 0)
//         return it->second.begin()->second;

//     auto it2 = it->second.find(assetId);
//     if (it2 == it->second.end()) 
//         return NULL;
//     return it2->second;
// }


// // unsigned int CInstanceManager::AddEntity()
// // {
// //     unsigned int id = GetUniqueId();
// //     AddEntityId(id);
// //     return id;
// // }

// // IComponent* CInstanceManager::GetComponentOfType(CString typeName, unsigned int componentId)
// // {
// //     auto it = Components.find(typeName.GetStdString());
// //     if (it == Components.end())
// //         return NULL;
// //     if (it->second.begin() == it->second.end())
// //         return NULL;
// //     if (componentId == 0)
// //         return it->second.begin()->second;

// //     auto it2 = it->second.find(componentId);
// //     if (it2 == it->second.end()) 
// //         return NULL;
// //     return it2->second;
// // }

// // void CInstanceManager::AddEntityId(unsigned int entityId)
// // {
// //     Ids.push_back(entityId);
// // }

// // void CInstanceManager::GetAllComponents(std::vector<IComponent*> &components)
// // {
// //     auto it = Components.begin();
// //     for (it; it != Components.end(); it++)
// //     {
// //         auto it2 = it->second.begin();
// //         for (it2; it2 != it->second.end(); it2++)
// //         {
// //             components.push_back(it2->second);
// //         }
// //     }
// // }
