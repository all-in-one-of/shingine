#include "Solver/CSolver.h"
#include "Solver/CSolver_Impl.h"

CSolver::CSolver()
{
    solver = new CSolver_Impl();
}

void CSolver::AddSystem(const CString& systemTypeName)
{
    solver->AddSystem(systemTypeName);
}

void CSolver::SetActive(bool active, const CString& systemTypeName)
{
    solver->SetActive(active, systemTypeName);
}

void CSolver::InitializeSystems()
{
    solver->InitializeSystems();
}

bool CSolver::Simulate()
{
    return solver->Simulate();
}

bool CSolver::IsValid()
{
    return solver->IsValid();
}
