#pragma once
#include <string>
#include <unordered_map>
#include "ISystem.h"
#include "Core.h"

class IComponent;
class CTransformSystem : public ISystem
{
public:
    virtual ~CTransformSystem() {};
    virtual void Initialize();
    virtual void Update();
private:
    void CalculateTransforms(bool ignoreStatic=true);
    glm::mat4 CalculateTransform(IComponent* transformComponent, 
        std::unordered_map<std::string, std::unordered_map<unsigned int, IComponent*>>::iterator &transformCollectionIterator, bool ignoreStatic);
};
