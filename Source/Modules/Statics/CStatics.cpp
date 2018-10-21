#include "Modules/Statics/CStatics.h"
#include "Utility/Data/ISerialized.h"

#include "Engine/IComponent.h"
#include "Modules/Statics/CAssetManager.h"
#include "Modules/Statics/CComponentManager.h"

CStatics* CStatics::Instance = NULL;

CStatics::CStatics()
{
    // unsigned int maxItems = 10000000;
    // unsigned int startId = 1000;
    // IdPool = std::vector<unsigned int>(maxItems - 1000);

    // for (unsigned int x = 0; x < IdPool.size(); x++)
    //     IdPool[x] = x + startId;
}

unsigned int CStatics::GetUniqueId()
{
    if (IdPool.size() == 0)
    {
        return NextId++;
    }
    unsigned int id = IdPool[IdPool.size() - 1];
    IdPool.pop_back();
    return id;
}

void CStatics::Destroy(ISerializedClass *object)
{
    
    // TODO
    // if component delete from map by name + entity id
    // if asset delete by unique id

    IdPool.push_back(object->UniqueID());
    delete object;
}


void CStatics::AddSerializedObject(ISerializedClass* object)
{
    IComponent* component = dynamic_cast<IComponent*>(object);
    if (component)
        CComponentManager::Get()->AddComponent(component);
    else
        CAssetManager::Get()->AddInstance(object);
}
