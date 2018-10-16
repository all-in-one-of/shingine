#pragma once
#include "IScene.h"

class IDataNode;
class CSceneMaker
{
public:
    static IScene* Create(const std::vector<IDataNode*> &nodes);
};
