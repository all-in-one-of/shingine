#pragma once
#include "IComponentMap.h"
#include "ISerialized.h"
#include <unordered_map>

template <class T>
class ComponentMap : public IComponentMap, public ISerialized {
public:
  ComponentMap() {}
  virtual ~ComponentMap() {}
  virtual String SerializedName() { return "ComponentMap"; }
  virtual String TypeName() { return "CoÎmponentMap"; }

private:
  std::unordered_map<unsigned int, T *> Components;
};
