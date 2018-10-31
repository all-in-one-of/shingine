#pragma once
#include "IComponentManager.h"
#include "Utility/Data/Serialization.h"
#include <typeinfo>
#include <unordered_map>

class IComponent;

class ComponentManager : public IComponentManager, public ISerializedClass {
public:
  SERIALIZE_CLASS(ComponentManager);
  ComponentManager();
  virtual ~ComponentManager() {}

  virtual void AddComponent(IComponent *component);
  virtual IComponent *AddComponent(String type, unsigned int entityId = 0);
  virtual void UpdateComponentEntityId(IComponent *component);
  virtual IComponent *GetComponentOfType(String typeName,
                                         unsigned int entityId = 0);

  virtual void GetAllComponents(std::vector<IComponent *> &components);
  virtual bool GetComponentIteratorOfType(String typeName,
                                          StringMap::iterator &iterator);

private:
  StringMap Components;
};
