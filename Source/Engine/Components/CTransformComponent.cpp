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

glm::vec3 CTransformComponent::GetLocalPosition()
{
    return glm::vec3(LocalPosition[0], LocalPosition[1], LocalPosition[2]);
}
glm::quat CTransformComponent::GetLocalRotation()
{
    return glm::quat(LocalRotation[0], LocalRotation[1], LocalRotation[2], LocalRotation[3]);
}
glm::vec3 CTransformComponent::GetLocalScale()
{
    return glm::vec3(LocalScale[0], LocalScale[1], LocalScale[2]);
}

void CTransformComponent::SetLocalPosition(float x, float y, float z)
{
    LocalPosition = {x, y, z};
}

void CTransformComponent::SetLocalPosition(glm::vec3 &val)
{
    LocalPosition = {val.x, val.y, val.z};
}
void CTransformComponent::SetLocalRotation(glm::quat &val)
{
    LocalRotation = {val.x, val.y, val.z, val.w};
}
void CTransformComponent::SetLocalScale(glm::vec3 &val)
{
    LocalScale = {val.x, val.y, val.z};
}
