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

    glm::vec3 GetLocalPosition();
    glm::quat GetLocalRotation();
    glm::vec3 GetLocalScale();

    void SetLocalPosition(float x, float y, float z);
    void SetLocalPosition(glm::vec3 &val);
    void SetLocalRotation(glm::quat &val);
    void SetLocalScale(glm::vec3 &val);

    // cached values
    glm::mat4 LocalTransform;
    glm::mat4 WorldTransform;
    glm::mat4 WorldTransformInv;
    glm::mat4 WorldTransformUniformScale;

    glm::vec3 Position;
    glm::quat Rotation;
    glm::vec3 Scale;
private:
    ATTRIBUTE_VECTOR(float, LocalPosition)
    ATTRIBUTE_VECTOR(float, LocalScale)
    ATTRIBUTE_VECTOR(float, LocalRotation)
};
