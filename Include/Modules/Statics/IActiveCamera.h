#pragma once
#include "Statics.h"
#include <glm/glm.hpp>

class CameraComponent;
class TransformComponent;
class IActiveCamera {
public:
  virtual ~IActiveCamera(){};
  virtual CameraComponent *GetCameraComponent() = 0;
  virtual TransformComponent *GetTransformComponent() = 0;
  virtual glm::mat4 ProjectionMatrix() = 0;
  virtual glm::mat4 ViewMatrix() = 0;
};
