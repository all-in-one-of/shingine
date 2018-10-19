#pragma once
#include <map>
#include "ISystem.h"

class CTransformComponent;
class CTransformSystem : public ISystem
{
public:
    virtual ~CTransformSystem() {};
    virtual void Initialize();
    virtual void Update();
private:
    void UpdateComponentMap();
    void CalculateTransforms(bool ignoreStatic=true);
    void CalculateTransform(CTransformComponent* transform);
    std::map<unsigned int, CTransformComponent*> Transforms;
};
