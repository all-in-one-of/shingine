#pragma once
#include "IEventSystem.h"
#include "Utility/Data/Serialization.h"

class EventSystem : public IEventSystem, public IObject {
public:
  SERIALIZE_CLASS(EventSystem);
  EventSystem();
  virtual ~EventSystem() {}
  virtual void AddDelegate(EventType type, class Delegate delegate);
  virtual void DispatchEvent(EventType type);
private:
  std::unordered_map<int, Event> Events;
};
