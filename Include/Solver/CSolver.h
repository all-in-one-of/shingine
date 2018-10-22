#pragma once

class CSolver_Impl;
class CString;

class CSolver
{
public:
    CSolver();
    void AddSystem(const CString& systemTypeName);
    void SetActive(bool active, const CString& systemTypeName);
    void InitializeSystems();
    bool Simulate();
    bool IsValid();
private:
    CSolver_Impl* solver;
};
