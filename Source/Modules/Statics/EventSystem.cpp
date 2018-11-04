#include "Modules/Statics/EventSystem.h"
REGISTER_SERIALIZED_CLASS(EventSystem)

EventSystem::EventSystem() {}
typedef std::unordered_map<int, Event> EventMap;
void EventSystem::AddDelegate(EventType type, Delegate delegate) {
  EventMap::iterator eventIterator = Events.find((int)type);
  if (eventIterator == Events.end()) {
    Events[(int)type] = Event(type);
  }
  Events[type].Add(delegate);
}

void EventSystem::DispatchEvent(EventType type) {
   EventMap::iterator eventIterator = Events.find((int)type);
   Events[(int)type].Dispatch();
}
