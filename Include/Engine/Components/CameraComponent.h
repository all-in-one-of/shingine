#pragma once
#include "ComponentSetup.h"
#include "Utility/Typedefs.h"

class CameraComponent : public Component {
public:
  COMPONENT_CLASS(CameraComponent);
  CameraComponent() {
    ATTRIBUTE_REGISTER(CameraComponent, FOV);
    ATTRIBUTE_REGISTER(CameraComponent, NearPlane);
    ATTRIBUTE_REGISTER(CameraComponent, FarPlane);

    FOV = .6f;
    NearPlane = .1f;
    FarPlane = 1000.f;

    ViewMatrix = glm::mat4(1);
    ProjectionMatrix = glm::mat4(1);
  }
  float GetFOV() { return FOV; }
  float GetNearPlane() { return NearPlane; }
  float GetFarPlane() { return FarPlane; }
  bool IsCurrent() { return Current; }
  void SetCurrent(bool current) { Current = current; }

  glm::mat4 GetViewMatrix() { return ViewMatrix; }
  glm::mat4 GetProjectionMatrix() { return ProjectionMatrix; }
  void SetViewMatrix(const glm::mat4 &mat) { ViewMatrix = mat; }
  void SetProjectionMatrix(const glm::mat4 &mat) { ProjectionMatrix = mat; }

private:
  ATTRIBUTE_VALUE(float, FOV);
  ATTRIBUTE_VALUE(float, NearPlane);
  ATTRIBUTE_VALUE(float, FarPlane);

  bool Current = true;

  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;
};
