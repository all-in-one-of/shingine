#pragma once
#include <string>
#include <unordered_map>
#include "Core.h"
#include "CSystem.h"
#include "Utility/Data/Serialization.h"

class IComponent;
class CTransformSystem : public CSystem, public ISerializedClass
{
public:
    SERIALIZE_CLASS(CTransformSystem)
    CTransformSystem(){};
    virtual ~CTransformSystem() {};
    virtual bool Initialize();
    virtual bool Update();
private:
    void CalculateTransforms(bool ignoreStatic=true);
    glm::mat4 CalculateTransform(IComponent* transformComponent, 
        std::unordered_map<std::string, std::unordered_map<unsigned int, IComponent*>>::iterator &transformCollectionIterator, bool ignoreStatic);
};
