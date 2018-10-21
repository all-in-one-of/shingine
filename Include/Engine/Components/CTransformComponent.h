#pragma once
#include "ComponentSetup.h"
#include "Core.h"

class CTransformComponent : public CComponent
{
public:
    SERIALIZE_CLASS(CTransformComponent)
    CTransformComponent()
    {
        ATTRIBUTE_REGISTER(CTransformComponent, ParentID)
        ATTRIBUTE_REGISTER(CTransformComponent, IsDynamic)
        ATTRIBUTE_REGISTER(CTransformComponent, LocalPosition)
        ATTRIBUTE_REGISTER(CTransformComponent, LocalScale)
        ATTRIBUTE_REGISTER(CTransformComponent, LocalRotation)
    }
    virtual ~CTransformComponent() {};

    ATTRIBUTE_ID(ParentID)
    ATTRIBUTE_VALUE(unsigned char, IsDynamic)
    ATTRIBUTE_VECTOR(float, LocalPosition)
    ATTRIBUTE_VECTOR(float, LocalScale)
    ATTRIBUTE_VECTOR(float, LocalRotation)

    glm::vec3 GetLocalPosition();
    glm::quat GetLocalRotation();
    glm::vec3 GetLocalScale();

    void SetLocalPosition(glm::vec3 &val);
    void SetLocalRotation(glm::quat &val);
    void SetLocalScale(glm::vec3 &val);

    // cached values
    glm::mat4 LocalTransform;
    glm::mat4 WorldTransform;
    glm::mat4 WorldTransformUniformScale;

    glm::vec3 Position;
    glm::quat Rotation;
    glm::vec3 Scale;
};
