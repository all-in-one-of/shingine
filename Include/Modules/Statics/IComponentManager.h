#pragma once
#include "Statics.h"
#include "Utility/Data/ComponentMap.h"
#include <unordered_map>

class IComponent;

class IComponentManager {
public:
  virtual ~IComponentManager(){};

  virtual void AddGenericComponent(IComponent *component) = 0;
  virtual IComponent *AddComponent(String type, unsigned int entityId = 0) = 0;
  virtual void UpdateComponentEntityId(IComponent *component) = 0;
  virtual void GetAllComponents(std::vector<IComponent *> &components) = 0;
  virtual IComponentMap *GetComponentMap(const String &typeName) = 0;
  virtual void DestroyComponent(IComponent *component) = 0;
  virtual void
  GetComponentsForEntity(unsigned int entityId,
                         std::vector<IComponent *> &components) = 0;

  template <class T> ComponentMap<T> *GetComponentMap() {
    String typeName = typeid(T).name();
    SerializedFactory::GetDemangledName(typeName);
    IComponentMap *componentMap = GetComponentMap(typeName);
    return dynamic_cast<ComponentMap<T> *>(componentMap);
  }

  template <class T> T *GetComponentOfType(unsigned int entityId = 0) {
    ComponentMap<T> *componentMap = GetComponentMap<T>();
    if (!componentMap)
      return nullptr;
    if (entityId == 0)
      return componentMap->AtIndex(0);
    return componentMap->At(entityId);
  }

  template <class T> T *AddComponent(unsigned int entityId = 0) {
    String typeName = typeid(T).name();
    SerializedFactory::GetDemangledName(typeName);
    IComponent *component = AddComponent(typeName, entityId);
    return dynamic_cast<T *>(component);
  }
};
