#include "Modules/Statics/EntityManager.h"
#include "Engine/IComponent.h"
#include "Modules/Statics/IComponentManager.h"
#include <algorithm>
#include <iterator>

REGISTER_SERIALIZED_CLASS(EntityManager)

EntityManager::EntityManager() {}
unsigned int EntityManager::CreateEntity() {
  return CreateEntity(Statics::GetUniqueId());
}

unsigned int EntityManager::CreateEntity(unsigned int id) {
  Ids.push_back(id);
  return id;
}

unsigned int
EntityManager::CreateEntity(std::vector<String> componentsToAttach) {
  unsigned int id = CreateEntity();
  for (size_t x = 0; x < componentsToAttach.size(); x++) {
    Statics::Get<IComponentManager>()->AddComponent(componentsToAttach[x], id);
  }
  return id;
}

void EntityManager::Destroy(unsigned int id) {
  std::vector<unsigned int>::iterator it =
      std::find(Ids.begin(), Ids.end(), id);

  int deleteIndex = -1;
  for (int x = 0; x < Ids.size(); x++) {
    if (Ids[x] == id) {
      deleteIndex = x;
      break;
    }
  }
  DestroyAtIndex(std::distance(Ids.begin(), it));
}

unsigned int EntityManager::GetEntityCount() { return Ids.size(); }
unsigned int EntityManager::GetEntityIdAtIndex(unsigned int index) {
  return Ids[index >= Ids.size() ? Ids.size() - 1 : index];
}

void EntityManager::DestroyAtIndex(unsigned int index) {
  // get unique id
  unsigned int uid = Ids[index];
  // go to component manager
  std::vector<IComponent *> components;
  IComponentManager *componentManager = Statics::Get<IComponentManager>();
  componentManager->GetComponentsForEntity(uid, components);
  for (unsigned int x = 0; x < components.size(); x++)
    componentManager->DestroyComponent(components[x]);
  // return entity id
  Statics::ReturnUniqueId(uid);
  Ids.erase(Ids.begin() + index);
}

void EntityManager::DestroyAllEntities(bool keepPersistent) {
  // first delete persistent entities from the main vector
  for (unsigned int x = 0; x < PersistentEntityIds.size(); x++) {
    std::vector<unsigned int>::iterator it =
        std::find(Ids.begin(), Ids.end(), PersistentEntityIds[x]);
    Ids.erase(it);
  }
  for (; Ids.size() > 0;)
    DestroyAtIndex(0);
  // persistent ids back
  for (unsigned int x = 0; x < PersistentEntityIds.size(); x++)
    Ids.push_back(PersistentEntityIds[x]);
}

void EntityManager::SetPersistentEntity(unsigned int entityId, bool state) {
  std::vector<unsigned int>::iterator it = std::find(
      PersistentEntityIds.begin(), PersistentEntityIds.end(), entityId);
  if (it == PersistentEntityIds.end() && state == true)
    PersistentEntityIds.push_back(entityId);
  else {
    if (state == false)
      PersistentEntityIds.erase(it);
  }
}
