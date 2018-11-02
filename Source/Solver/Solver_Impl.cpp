#include "Solver/Solver_Impl.h"
#include "Systems/ISystem.h"
#include "Utility/Data/SerializedFactory.h"
#include <string>

Solver_Impl::Solver_Impl() {}

void Solver_Impl::AddSystem(const String &systemTypeName) {
  std::string typeName = systemTypeName;
  ISerialized *newSystem = SerializedFactory::CreateInstance(typeName);
  if (!newSystem)
    return;
  Systems[typeName] = dynamic_cast<ISystem *>(newSystem);
}

void Solver_Impl::SetActive(bool active, const String &systemTypeName) {
  ISystem *sys = Systems.at(systemTypeName);
  if (!sys)
    return;
  sys->SetActive(active);
}

void Solver_Impl::InitializeSystems() {
  for (SystemsMap::iterator it = Systems.begin(); it != Systems.end(); it++) {
    if (!it->second->Initialize()) {
      Valid = false;
      return;
    }
  }
  Valid = true;
}

bool Solver_Impl::Simulate() {
  if (!Valid)
    return false;

  bool solverActive = true;
  for (SystemsMap::iterator it = Systems.begin(); it != Systems.end(); it++) {
    solverActive = solverActive && it->second->Update();
  }
  return solverActive;
}

bool Solver_Impl::IsValid() { return Valid; }
