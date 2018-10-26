#include "Engine/Components/CTransformComponent.h"
REGISTER_SERIALIZED_NAME(CTransformComponent,Transform)

CTransformComponent::CTransformComponent()
{
    ATTRIBUTE_REGISTER(CTransformComponent, ParentID)
    ATTRIBUTE_REGISTER(CTransformComponent, IsDynamic)
    ATTRIBUTE_REGISTER(CTransformComponent, LocalPosition)
    ATTRIBUTE_REGISTER(CTransformComponent, LocalScale)
    ATTRIBUTE_REGISTER(CTransformComponent, LocalRotation)
    LocalPosition = {0.f, 0.f, 0.f};
    LocalRotation = {0.f, 0.f, 0.f, 1.f}; // 0 quat
    LocalScale = {1.f, 1.f, 1.f};
}

glm::vec3 CTransformComponent::GetPosition()
{
    return glm::vec3(LocalPosition[0], LocalPosition[1], LocalPosition[2]);
}
glm::quat CTransformComponent::GetRotation()
{
    glm::quat q;
    q.x = LocalRotation[0];
    q.y = LocalRotation[1]; 
    q.z = LocalRotation[2];
    q.w = LocalRotation[3];
    return q;
}
glm::vec3 CTransformComponent::GetScale()
{
    return glm::vec3(LocalScale[0], LocalScale[1], LocalScale[2]);
}

void CTransformComponent::SetPosition(float x, float y, float z)
{
    LocalPosition = {x, y, z};
}

void CTransformComponent::SetPosition(glm::vec3 &val)
{
    LocalPosition = {val.x, val.y, val.z};
}
void CTransformComponent::SetRotation(glm::quat &val)
{
    LocalRotation = {val.x, val.y, val.z, val.w};
}
void CTransformComponent::SetScale(glm::vec3 &val)
{
    LocalScale = {val.x, val.y, val.z};
}
