#pragma once
#include "Statics.h"
#include <unordered_map>

class IComponent;

class IComponentManager {
public:
  typedef std::unordered_map<unsigned int, IComponent *> IdMap;
  typedef std::unordered_map<std::string, IdMap> StringMap;

  virtual ~IComponentManager(){};

  virtual void AddComponent(IComponent *component) = 0;
  virtual IComponent *AddComponent(String type, unsigned int entityId = 0) = 0;
  virtual void UpdateComponentEntityId(IComponent *component) = 0;
  virtual IComponent *GetComponentOfType(String typeName,
                                         unsigned int entityId = 0) = 0;

  virtual void GetAllComponents(std::vector<IComponent *> &components) = 0;
  virtual bool GetComponentIteratorOfType(String typeName,
                                          StringMap::iterator &iterator) = 0;

  template <class T> T *GetComponentOfType(unsigned int entityId = 0) {
    String typeName = typeid(T).name();
    SerializedFactory::GetDemangledName(typeName);
    IComponent *component = GetComponentOfType(typeName, entityId);
    return dynamic_cast<T *>(component);
  }

  template <class T> T *AddComponent(unsigned int entityId = 0) {
    String typeName = typeid(T).name();
    SerializedFactory::GetDemangledName(typeName);
    IComponent *component = AddComponent(typeName, entityId);
    return dynamic_cast<T *>(component);
  }
};
