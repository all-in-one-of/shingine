#include "Utility/Data/CSerializedFactory.h"
#include "Modules/Statics/CInstanceManager.h"

CSerializedFactory::TSerializedTypeMap* CSerializedFactory::Map = NULL;

ISerialized* CSerializedFactory::CreateInstance(const std::string &s)
{
    TSerializedTypeMap::iterator it = GetMap()->find(s);
    if (it == GetMap()->end()) 
        return NULL;

    ISerialized* createdInstance = it->second();
    ISerializedClass* serializedClass = dynamic_cast<ISerializedClass*>(createdInstance);
    // set unique id
    unsigned int uniqueId = CInstanceManager::Get()->GetUniqueId();
    serializedClass->SetUniqueID(uniqueId);
    // add to the instance manager
    CInstanceManager::Get()->AddInstance(serializedClass);
    return createdInstance;
}

CSerializedFactory::TSerializedTypeMap* CSerializedFactory::GetMap()
{
    if (!Map) 
        Map = new TSerializedTypeMap;
    return Map;
}
