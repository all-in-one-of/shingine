#include "Modules/Statics/Statics.h"
#include "Utility/Data/ISerialized.h"

#include "Engine/IComponent.h"
#include "Modules/Statics/AssetManager.h"
#include "Modules/Statics/ComponentManager.h"

Statics* Statics::Instance = NULL;

Statics::Statics()
{
    // unsigned int maxItems = 10000000;
    // unsigned int startId = 1000;
    // IdPool = std::vector<unsigned int>(maxItems - 1000);

    // for (unsigned int x = 0; x < IdPool.size(); x++)
    //     IdPool[x] = x + startId;
}

unsigned int Statics::GetUniqueId()
{
    if (IdPool.size() == 0)
    {
        return NextId++;
    }
    unsigned int id = IdPool[IdPool.size() - 1];
    IdPool.pop_back();
    return id;
}

void Statics::Destroy(ISerializedClass *object)
{
    
    // TODO
    // if component delete from map by name + entity id
    // if asset delete by unique id

    IdPool.push_back(object->UniqueID());
    delete object;
}


void Statics::AddSerializedObject(ISerializedClass* object)
{
    IComponent* component = dynamic_cast<IComponent*>(object);
    if (component)
        ComponentManager::Get()->AddComponent(component);
    else
        AssetManager::Get()->AddInstance(object);
}
