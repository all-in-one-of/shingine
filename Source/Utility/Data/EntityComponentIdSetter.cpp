#include "Utility/Data/EntityComponentIdSetter.h"
#include "Engine/Entity.h"
#include "Engine/IComponent.h"
#include "Utility/Data/ISerialized.h"
#include <unordered_map>

#include "Modules/Statics/IComponentManager.h"
#include "Modules/Statics/IEntityManager.h"

void EntityComponentIdSetter::UpdateIds(ISerialized *idCollections) {
  EntityIdCollection *collection =
      dynamic_cast<EntityIdCollection *>(idCollections);
  if (!collection)
    return;
  (void)EntityComponentIdSetter(collection);
}

EntityComponentIdSetter::EntityComponentIdSetter(
    EntityIdCollection *collection) {
  std::unordered_map<unsigned int, unsigned int> componentIdToEntityId;
  for (size_t x = 0; x < collection->Ids.size(); x++) {
    unsigned int entityId = collection->Ids[x];
    Statics::Get<IEntityManager>()->CreateEntity(entityId);
    for (size_t y = 0; y < collection->Components[x]->Ids.size(); y++)
      componentIdToEntityId[collection->Components[x]->Ids[y]] = entityId;
  }

  std::vector<IComponent *> components;
  Statics::Get<IComponentManager>()->GetAllComponents(components);
  for (size_t x = 0; x < components.size(); x++) {
    unsigned int componentId = components[x]->Id();
    std::unordered_map<unsigned int, unsigned int>::iterator it =
        componentIdToEntityId.find(componentId);
    if (it == componentIdToEntityId.end())
      continue;
    components[x]->SetEntityId(it->second);
    Statics::Get<IComponentManager>()->UpdateComponentEntityId(components[x]);
  }
}
