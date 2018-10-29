#include "Solver/Solver.h"
#include "Solver/Solver_Impl.h"

Solver::Solver()
{
    solver = new Solver_Impl();
}

void Solver::AddSystem(const String& systemTypeName)
{
    solver->AddSystem(systemTypeName);
}

void Solver::SetActive(bool active, const String& systemTypeName)
{
    solver->SetActive(active, systemTypeName);
}

void Solver::InitializeSystems()
{
    solver->InitializeSystems();
}

bool Solver::Simulate()
{
    return solver->Simulate();
}

bool Solver::IsValid()
{
    return solver->IsValid();
}
