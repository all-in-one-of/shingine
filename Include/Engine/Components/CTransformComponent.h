#pragma once
#include "ComponentSetup.h"
#include "Core.h"

class CTransformComponent : public CComponent, public ISerializedClass
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

    ATTRIBUTE_ID_REFERENCE(ParentID)
    ATTRIBUTE_VALUE_INFO(unsigned char, IsDynamic)
    ATTRIBUTE_VECTOR_INFO(float, LocalPosition)
    ATTRIBUTE_VECTOR_INFO(float, LocalScale)
    ATTRIBUTE_VECTOR_INFO(float, LocalRotation)

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
