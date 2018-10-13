#pragma once
#include "IScene.h"

namespace SSD { struct SNode; }
class CSceneMaker
{
public:
    static IScene* Create(SSD::SNode** nodes, const unsigned int &nodeCount);
};
