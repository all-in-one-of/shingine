#pragma once
#include "Statics.h"
#include "Event.h"

class IEventSystem {
public:
  virtual ~IEventSystem() {}
  virtual void AddDelegate(EventType type, class Delegate delegate) = 0;
  virtual void DispatchEvent(EventType type) = 0;
};
