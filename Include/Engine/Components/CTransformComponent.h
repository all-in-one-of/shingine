#pragma once
#include "ComponentSetup.h"
#include "../../Core.h"

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

    // cached values
    Matrix4x4 LocalTransform;
    Matrix4x4 WorldTransform;
    Matrix4x4 WorldTransformUniformScale;

    Vector3 Position;
    Quaternion Rotation;
    Vector3 Scale;
};
