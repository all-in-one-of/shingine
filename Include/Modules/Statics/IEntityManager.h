#pragma once
#include "Statics.h"
#include <vector>

class String;
class IEntityManager {
public:
  virtual ~IEntityManager() {}
  virtual unsigned int CreateEntity() = 0;
  virtual unsigned int CreateEntity(unsigned int entityId) = 0;
  virtual unsigned int CreateEntity(std::vector<String> componentsToAttach) = 0;
  virtual void Destroy(unsigned int entityId) = 0;
  virtual void DestroyAtIndex(unsigned int index) = 0;
  virtual void DestroyAllEntities(bool keepPersistent = true) = 0;
  virtual unsigned int GetEntityCount() = 0;
  virtual unsigned int GetEntityIdAtIndex(unsigned int index) = 0;
  virtual void SetPersistentEntity(unsigned int entityId,
                                   bool state = true) = 0;
};
