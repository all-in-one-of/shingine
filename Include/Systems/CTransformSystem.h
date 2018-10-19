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
    glm::mat4 CalculateTransform(CTransformComponent* transform, bool ignoreStatic=true);
    std::map<unsigned int, CTransformComponent*> Transforms;
};
