#include "CTransformComponent.h"
#include <iostream>

REGISTER_COMPONENT_NAME(CTransformComponent,LocalTransform);

CTransformComponent::CTransformComponent()
{
    std::cout << "Transform is created" << std::endl;
}

CTransformComponent::~CTransformComponent()
{
}
