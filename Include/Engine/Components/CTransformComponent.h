#pragma once
#include "ComponentSetup.h"
#include "Core.h"

class CTransformComponent : public CComponent
{
public:
    SERIALIZE_CLASS(CTransformComponent)
    CTransformComponent();
    virtual ~CTransformComponent() {};

    ATTRIBUTE_ID(ParentID)
    ATTRIBUTE_VALUE(unsigned char, IsDynamic)

    glm::vec3 GetPosition();
    glm::quat GetRotation();
    glm::vec3 GetScale();

    void SetPosition(float x, float y, float z);
    void SetPosition(glm::vec3 &val);
    void SetRotation(glm::quat &val);
    void SetScale(glm::vec3 &val);

    // cached values
    glm::mat4 LocalTransform;
    glm::mat4 LocalTransformUniformScale;

    glm::mat4 WorldTransform;
    glm::mat4 WorldTransformInv;
    glm::mat4 WorldTransformUniformScale;

    glm::vec3 WorldPosition;
    glm::quat WorldRotation;
    glm::vec3 WorldScale;
private:
    ATTRIBUTE_VECTOR(float, LocalPosition)
    ATTRIBUTE_VECTOR(float, LocalScale)
    ATTRIBUTE_VECTOR(float, LocalRotation)
};
