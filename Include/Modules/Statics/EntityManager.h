#pragma once
#include "Core.h"
#include "IEntityManager.h"
#include "Utility/Data/Serialization.h"
#include <vector>

class EntityManager : public IEntityManager, public IObject {
public:
  SERIALIZE_CLASS(EntityManager);
  EntityManager();
  virtual ~EntityManager() {}
  virtual unsigned int CreateEntity();
  virtual unsigned int CreateEntity(unsigned int entityId);
  virtual unsigned int CreateEntity(std::vector<String> componentsToAttach);
  virtual void Destroy(unsigned int entityId);
  virtual void DestroyAtIndex(unsigned int index);
  virtual void DestroyAllEntities(bool keepPersistent);
  virtual unsigned int GetEntityCount();
  virtual unsigned int GetEntityIdAtIndex(unsigned int index);
  virtual void SetPersistentEntity(unsigned int entityId, bool state);

private:
  static EntityManager *Instance;
  std::vector<unsigned int> Ids;
  std::vector<unsigned int> PersistentEntityIds;
};
