#include "Engine/Components/TransformComponent.h"
REGISTER_SERIALIZED_NAME(TransformComponent)

TransformComponent::TransformComponent()
{
    ATTRIBUTE_REGISTER(TransformComponent, ParentID)
    ATTRIBUTE_REGISTER(TransformComponent, IsDynamic)
    ATTRIBUTE_REGISTER(TransformComponent, LocalPosition)
    ATTRIBUTE_REGISTER(TransformComponent, LocalScale)
    ATTRIBUTE_REGISTER(TransformComponent, LocalRotation)
    LocalPosition = {0.f, 0.f, 0.f};
    LocalRotation = {0.f, 0.f, 0.f, 1.f}; // 0 quat
    LocalScale = {1.f, 1.f, 1.f};
}

glm::vec3 TransformComponent::GetPosition()
{
    return glm::vec3(LocalPosition[0], LocalPosition[1], LocalPosition[2]);
}
glm::quat TransformComponent::GetRotation()
{
    glm::quat q;
    q.x = LocalRotation[0];
    q.y = LocalRotation[1]; 
    q.z = LocalRotation[2];
    q.w = LocalRotation[3];
    return q;
}
glm::vec3 TransformComponent::GetScale()
{
    return glm::vec3(LocalScale[0], LocalScale[1], LocalScale[2]);
}

void TransformComponent::SetPosition(float x, float y, float z)
{
    LocalPosition = {x, y, z};
}

void TransformComponent::SetPosition(glm::vec3 &val)
{
    LocalPosition = {val.x, val.y, val.z};
}
void TransformComponent::SetRotation(glm::quat &val)
{
    LocalRotation = {val.x, val.y, val.z, val.w};
}
void TransformComponent::SetScale(glm::vec3 &val)
{
    LocalScale = {val.x, val.y, val.z};
}
