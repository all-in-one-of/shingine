#include "Systems/System.h"

void System::SetActive(bool active) { Active = active; }
bool System::IsActive() { return Active; }
bool System::Initialize() { return true; }
bool System::Update() { return true; }
