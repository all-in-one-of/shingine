#pragma once
#include "IComponentManager.h"
#include "Utility/Data/Serialization.h"
#include <typeinfo>
#include <unordered_map>
class IComponentMap;
class IComponent;

class ComponentManager : public IComponentManager, public IObject {
public:
  SERIALIZE_CLASS(ComponentManager);
  ComponentManager();
  virtual ~ComponentManager() {}

  virtual void AddGenericComponent(IComponent *component);
  virtual IComponent *AddComponent(String type, unsigned int entityId = 0);
  virtual void UpdateComponentEntityId(IComponent *component);

  virtual void GetAllComponents(std::vector<IComponent *> &components);
  virtual IComponentMap *GetComponentMap(const String &typeName);
  virtual void GetComponentsForEntity(unsigned int entityId,
                                      std::vector<IComponent *> &components);
  virtual void DestroyComponent(IComponent *component);

private:
  IComponentMap *AddComponentMap(const std::string &typeName);
  typedef std::unordered_map<std::string, IComponentMap *> StringCompMap;
  StringCompMap Components;
};
