#include "CTransformComponent.h"
#include <iostream>

REGISTER_COMPONENT_NAME(CTransformComponent,Transform)

CTransformComponent::CTransformComponent()
{
    std::cout << "Transform is created" << std::endl;
    ATTRIBUTE_REGISTER(CTransformComponent, Matrix)
}

CTransformComponent::~CTransformComponent()
{
}
