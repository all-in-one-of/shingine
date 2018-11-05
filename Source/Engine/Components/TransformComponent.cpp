#include "Engine/Components/TransformComponent.h"
REGISTER_COMPONENT(TransformComponent);

TransformComponent::TransformComponent() {
  ATTRIBUTE_REGISTER(TransformComponent, ParentID);
  ATTRIBUTE_REGISTER(TransformComponent, IsDynamic);
  ATTRIBUTE_REGISTER(TransformComponent, LocalPosition);
  ATTRIBUTE_REGISTER(TransformComponent, LocalScale);
  ATTRIBUTE_REGISTER(TransformComponent, LocalRotation);
  LocalPosition = {0.f, 0.f, 0.f};
  LocalRotation = {0.f, 0.f, 0.f, 1.f}; // 0 quat
  LocalScale = {1.f, 1.f, 1.f};
}

glm::vec3 TransformComponent::GetPosition() { return LocalPosition; }

void TransformComponent::GetPosition(glm::vec3 &position) {
  position = LocalPosition;
}

void TransformComponent::GetPosition(glm::vec4 &position) {
  position = glm::vec4(LocalPosition[0], LocalPosition[1], LocalPosition[2], 1);
}

glm::quat TransformComponent::GetRotation() { return LocalRotation; }
glm::vec3 TransformComponent::GetScale() { return LocalScale; }

void TransformComponent::SetPosition(float x, float y, float z) {
  LocalPosition = {x, y, z};
}

void TransformComponent::SetPosition(const glm::vec3 &val) {
  LocalPosition = val;
}
void TransformComponent::SetRotation(const glm::quat &val) {
  LocalRotation = val;
}
void TransformComponent::SetScale(const glm::vec3 &val) { LocalScale = val; }
