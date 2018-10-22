#pragma once
#include <unordered_map>
#include <string>
#include "CSolver.h"

class CSolver_Impl;
class CString;
class ISystem;

class CSolver_Impl
{
public:
    CSolver_Impl();
    void AddSystem(const CString& systemTypeName);
    void SetActive(bool active, const CString& systemTypeName);
    void InitializeSystems();
    bool Simulate();
    bool IsValid();
private:
    bool Valid = true;
    std::unordered_map<std::string, ISystem*> Systems;
};
