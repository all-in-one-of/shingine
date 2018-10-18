#pragma once
#include "IScene.h"

class CSceneMaker
{
public:
    static IScene* Create(const std::vector<IObject*> &objects);
};
