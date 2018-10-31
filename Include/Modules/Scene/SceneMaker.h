#pragma once
#include "IScene.h"

class SceneMaker {
public:
  static IScene *Create(const std::vector<IDataNode *> &nodes);
};
