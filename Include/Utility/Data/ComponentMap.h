#pragma once
#include "IComponentMap.h"
#include "ISerialized.h"
#include <unordered_map>
#include <vector>

template <class T>
class ComponentMap : public IComponentMap, public ISerialized {
public:
  typedef std::unordered_map<unsigned int, T *> IdMap;
  typedef typename IdMap::iterator IdMapIterator;

  ComponentMap() {}
  virtual ~ComponentMap() {}
  virtual unsigned int Count() { return Items.size(); }

  void Set(unsigned int entityId, IComponent *component) {
    T *newComponent = dynamic_cast<T *>(component);

    Components[entityId] = newComponent;
    Items.push_back(newComponent);
  }

  virtual void Erase(unsigned int entityId) {
    T *component = At(entityId);
    if (!component)
      return;
    int indexInVector = -1;
    for (int x = 0; x < Items.size(); x++) {
      if (Items[x]->EntityId() == entityId)
        indexInVector = x;
    }
    if (indexInVector != -1)
      Items.erase(Items.begin() + indexInVector);
    Components.erase(entityId);
  }

  virtual void GetAllComponents(std::vector<IComponent *> &componentVector) {
    for (IdMapIterator it = Components.begin(); it != Components.end(); it++)
      componentVector.push_back(dynamic_cast<IComponent *>(it->second));
  }

  T *AtIndex(unsigned int index) {
    if (index >= Count())
      return nullptr;
    return Items[index];
  }

  T *At(unsigned int entityId = 0) {
    IdMapIterator iterator = Components.find(entityId);
    if (iterator == Components.end())
      return nullptr;
    return iterator->second;
  }

  // ISerialized
  virtual String SerializedName() { return "ComponentMap"; }
  virtual String TypeName() { return "ComponentMap"; }

private:
  IdMap Components;
  std::vector<T *> Items;
};
