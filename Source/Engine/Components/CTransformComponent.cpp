#include "Engine/Components/CTransformComponent.h"
REGISTER_SERIALIZED_NAME(CTransformComponent,Transform)

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
