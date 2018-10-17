#include "CTransformComponent.h"
#include <iostream>

REGISTER_COMPONENT_NAME(CTransformComponent,Transform)

CTransformComponent::CTransformComponent()
{
    ATTRIBUTE_REGISTER(CTransformComponent, ParentID)
    ATTRIBUTE_REGISTER(CTransformComponent, Matrix)
}

CTransformComponent::~CTransformComponent()
{
}
