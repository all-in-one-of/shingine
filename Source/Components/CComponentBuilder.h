#pragma once
#include "IComponent.h"
#include "../Utility/SSD.h"

class CComponentBuilder
{
public:
    // Setup component from scene description attribute
    void Setup(IComponent* component, SSD::SAttribute* attr);
    // Setup component from scene description node (which can contain additional attributes)
    void Setup(IComponent* component, SSD::SNode* node);
};
