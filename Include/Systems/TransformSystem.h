#pragma once
#include "Core.h"
#include "System.h"
#include "Utility/Data/Serialization.h"
#include <string>
#include <unordered_map>

class IComponent;
class TransformComponent;
template <class T> class ComponentMap;
class TransformSystem : public System, public IObject {
public:
  SERIALIZE_CLASS(TransformSystem);
  TransformSystem(){};
  virtual ~TransformSystem(){};
  virtual bool Initialize();
  virtual bool Update();

private:
  void OnSceneReload();
  void CalculateLocalTransforms(bool ignoreStatic = true);
  void CalculateTransforms(bool ignoreStatic = true);
  typedef std::unordered_map<std::string,
                             std::unordered_map<unsigned int, IComponent *>>
      StringMap;
  void CalculateWorldTransforms(bool ignoreStatic);
  ComponentMap<TransformComponent> *TransformComponentMap;
};
