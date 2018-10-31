#pragma once
#include "ISystem.h"

class System : public ISystem {
public:
  virtual ~System(){};
  virtual void SetActive(bool active);
  virtual bool IsActive();

protected:
  bool Active = false;
};
