#pragma once
#include "../Object/IObject.h"

namespace SSD { struct SNode; }

class CSceneBuilder
{
public:
    void SetupObjectFromDescriptionNode(IObject* object, SSD::SNode* node);
};
