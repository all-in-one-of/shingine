#pragma once
#include "Core.h"
#include "IEntityManager.h"
#include "Utility/Data/Serialization.h"
#include <vector>

class EntityManager : public IEntityManager, public ISerializedClass {
public:
  SERIALIZE_CLASS(EntityManager);
  EntityManager();
  unsigned int CreateEntity();
  unsigned int CreateEntity(unsigned int id);
  unsigned int CreateEntity(std::vector<String> componentsToAttach);
  void Destroy(unsigned int id);

private:
  static EntityManager *Instance;
  std::vector<unsigned int> Ids;
};
