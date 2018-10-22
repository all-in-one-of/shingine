#include "Solver/CSolver_Impl.h"
#include "Utility/Data/CSerializedFactory.h"
#include "Systems/ISystem.h"
#include <string>

CSolver_Impl::CSolver_Impl()
{
}

void CSolver_Impl::AddSystem(const CString& systemTypeName)
{
    std::string typeName = systemTypeName.GetStdString();
    ISerialized* newSystem = CSerializedFactory::CreateInstance(typeName);
    if (!newSystem)
        return;
    Systems[typeName] = dynamic_cast<ISystem*>(newSystem);
}

void CSolver_Impl::SetActive(bool active, const CString& systemTypeName)
{
    ISystem* sys = Systems.at(systemTypeName.GetStdString());
    if (!sys)
        return;
    sys->SetActive(active);
}

void CSolver_Impl::InitializeSystems()
{
    for (auto it = Systems.begin(); it != Systems.end(); it++)
    {
        if(!it->second->Initialize())
        {
            Valid = false;
            return;
        }
    }
    Valid = true;
}

bool CSolver_Impl::Simulate()
{
    if (!Valid) 
        return false;
    
    bool solverActive = true;
    for (auto it = Systems.begin(); it != Systems.end(); it++)
    {
        solverActive = solverActive && it->second->Update();
    }
    return solverActive;
}

bool CSolver_Impl::IsValid()
{
    return Valid;
}
