#include "CTransformComponent.h"
#include <iostream>

REGISTER_COMPONENT_NAME(CTransformComponent,Transform);

CTransformComponent::CTransformComponent()
{
    std::cout << "Transform is created" << std::endl;
}

CTransformComponent::~CTransformComponent()
{
}
