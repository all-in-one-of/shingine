#include "CTransformComponent.h"

REGISTER_SERIALIZED_NAME(CTransformComponent,Transform)

CTransformComponent::CTransformComponent()
{
    ATTRIBUTE_REGISTER(CTransformComponent, ParentID)
    ATTRIBUTE_REGISTER(CTransformComponent, Matrix)
}
