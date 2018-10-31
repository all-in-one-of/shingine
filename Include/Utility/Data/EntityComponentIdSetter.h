#pragma once
#include <map>
class ISerialized;
class EntityIdCollection;
class EntityComponentIdSetter {
public:
  static void UpdateIds(ISerialized *idCollections);

private:
  EntityComponentIdSetter(){};
  EntityComponentIdSetter(EntityIdCollection *collection);
};
