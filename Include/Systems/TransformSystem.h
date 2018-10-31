#pragma once
#include "Core.h"
#include "System.h"
#include "Utility/Data/Serialization.h"
#include <string>
#include <unordered_map>

class IComponent;
class TransformSystem : public System, public ISerializedClass {
public:
  SERIALIZE_CLASS(TransformSystem);
  TransformSystem(){};
  virtual ~TransformSystem(){};
  virtual bool Initialize();
  virtual bool Update();

private:
  void CalculateLocalTransforms(bool ignoreStatic = true);
  void CalculateTransforms(bool ignoreStatic = true);
  glm::mat4 GetRotationMatrix(glm::quat q);
  typedef std::unordered_map<std::string,
                             std::unordered_map<unsigned int, IComponent *>>
      StringMap;
  void CalculateWorldTransforms(bool ignoreStatic);
  StringMap::iterator TransformCollectionIterator;
};
