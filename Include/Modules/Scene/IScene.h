#pragma once
#include "Common.h"
#include "Utility/Data/IDataNode.h"
#include <vector>

class IScene // : public IObject
{
public:
  virtual ~IScene(){};
  virtual void Save() = 0;
  virtual void SetSerializedData(const std::vector<IDataNode *> &nodes) = 0;
};
