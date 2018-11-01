#pragma once
#include <vector>
class IComponent;
class IComponentMap {
public:
  virtual ~IComponentMap() {}
  virtual void Set(unsigned int entityId, IComponent *component) = 0;
  virtual void Erase(unsigned int entityId) = 0;
  virtual void GetAllComponents(std::vector<IComponent *> &componentVector) = 0;
  virtual unsigned int Count() = 0;
};