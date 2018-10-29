#pragma once

class Solver_Impl;
class String;

class Solver
{
public:
    Solver();
    void AddSystem(const String& systemTypeName);
    void SetActive(bool active, const String& systemTypeName);
    void InitializeSystems();
    bool Simulate();
    bool IsValid();
private:
    Solver_Impl* solver;
};
