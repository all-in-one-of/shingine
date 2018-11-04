#pragma once
#include "Utility/Data/Serialization.h"
#include <unordered_map>

class Entity : public IObject {
public:
  SERIALIZE_CLASS(Entity);
  Entity(){};
  virtual ~Entity(){};
};

class ComponentIdCollection : public IObject {
public:
  SERIALIZE_CLASS(ComponentIdCollection);
  ComponentIdCollection() { ATTRIBUTE_REGISTER(ComponentIdCollection, Ids); };
  virtual ~ComponentIdCollection(){};
  ATTRIBUTE_ID_VECTOR(Ids);
};

class EntityIdCollection : public IObject {
public:
  SERIALIZE_CLASS(EntityIdCollection);
  EntityIdCollection() {
    ATTRIBUTE_REGISTER(EntityIdCollection, Ids);
    ATTRIBUTE_REGISTER(EntityIdCollection, Components);
  };
  virtual ~EntityIdCollection() {
    for (size_t x = 0; x < Components.size(); x++)
      delete Components[x];
  };
  ATTRIBUTE_ID_VECTOR(Ids);
  ATTRIBUTE_CLASS_VECTOR(ComponentIdCollection, Components);
};
