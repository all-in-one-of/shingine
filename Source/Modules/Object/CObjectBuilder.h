#pragma once
#include "IObject.h"

namespace SSD { struct SNode; }

class CObjectBuilder
{
public:
    void SetupObjectFromDescriptionNode(IObject* object, SSD::SNode* node);
};
