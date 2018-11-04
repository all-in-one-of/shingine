#pragma once
#include <unordered_map>
#include <vector>

class Delegate {
  template <class Class, void (Class::*Function)()>
  static void ClassFunctionWrapper(void *instance) {
    return (static_cast<Class *>(instance)->*Function)();
  }
public:
  typedef void (*Function)(void *);
  template <class Class, void (Class::*ClassFunction)()>
  void SetFunction(Class *instance) {
    Instance = instance;
    DelegateFunction = ClassFunctionWrapper<Class, ClassFunction>;
  }
  void Invoke() { DelegateFunction(Instance); }
private:
  void *Instance;
  Function DelegateFunction;
};

enum EventType { OnInitialize, OnSceneLoad };
class Event {
public:
  Event() {}
  Event(EventType type) { EventType = type; }
  void Add(Delegate delegate) { Delegates.push_back(delegate); }
  void Remove(Delegate delegate) { throw 1; } // to do
  void Dispatch() {
    for (unsigned int x = 0; x < Delegates.size(); x++)
      Delegates[x].Invoke();
  }

private:
  EventType EventType;
  std::vector<Delegate> Delegates;
};
