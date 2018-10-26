#include "Utility/Data/CSerializedFactory.h"
#include "Modules/Statics/CStatics.h"

CSerializedFactory::TSerializedTypeMap* CSerializedFactory::Map = NULL;

ISerialized* CSerializedFactory::CreateInstance(const std::string &s, bool setUid)
{
    TSerializedTypeMap::iterator it = GetMap()->find(s);
    if (it == GetMap()->end()) 
        return NULL;

    ISerialized* createdInstance = it->second();
    ISerializedClass* serializedClass = dynamic_cast<ISerializedClass*>(createdInstance);
    // set unique id
    if (setUid)
    {
        unsigned int uniqueId = CStatics::Get()->GetUniqueId();
        serializedClass->SetUniqueID(uniqueId);

        // initialize classes
        std::vector<ISerialized*> serializedAttributes;
        serializedClass->GetAllAttributes(serializedAttributes);
    }
    // add to the instance manager
    return createdInstance;
}

CSerializedFactory::TSerializedTypeMap* CSerializedFactory::GetMap()
{
    if (!Map) 
        Map = new TSerializedTypeMap;
    return Map;
}
